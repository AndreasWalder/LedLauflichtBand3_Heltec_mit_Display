#include "INI_Setup_html.h"
File fsUploadFile;

void  analyzeZeile ( const char* str ) ;
void handleFileUpload();
void DatenLaden (String argument, String Wert);

void Einstellen()   // INI wifimanager im Access Point Modus
{
  WiFi.mode(WIFI_AP);
  IPAddress apIP(192, 168, 4, 1);    // Hier wird IP bestimmt
  //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Led-Panel-Setup", "Admin");  // Name und Passwort des Access Points
  delay(100);
  if (SerialDebug == 1)  Serial.println("Starte AP  Led-Panel-Setup");
  if (SerialDebug == 1)  Serial.print("IP Adresse ");      //Ausgabe aktueller IP des Servers
  if (SerialDebug == 1)  Serial.println(WiFi.softAPIP());

  server.on("/edit", []() {
    String  content;
    if (SerialDebug == 1) Serial.print("edit");
    if (server.hasArg("content"))   // setup.ini vom wifimanager Webseite speichern
    {
      content = server.arg("content");
      if (SerialDebug == 1) Serial.print(content);
      File DataFile = SPIFFS.open("/setup.ini", "w");
      DataFile.print(content);
      DataFile.close();
      return;
    }
    if (server.hasArg("reset"))     // ESP32 Neustarten wenn http://192.168.4.1/edit?reset=0
    {
      server.send(404, "text/plain", "Reset");
      delay(100);
      ESP.restart() ;
      return;
    }
    server.send(404, "text/plain", "Antwort");
  });

  server.on("/setup.ini", []() {         // setup.ini an Browser senden
    File file = SPIFFS.open("/setup.ini", "r");
    server.sendHeader("Cache-Control", "no-cache");
    size_t sent = server.streamFile(file, "text/plain");
    file.close();
  });

  server.on("/", []() {                 // INI wifimanager Index Webseite senden
    server.send(200, "text/html", INI_Setup_html);
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "FileNotFound");
  });

  server.on("/upload", HTTP_POST, []() {  // INI - Datei in ESP32 hochladen
    server.send(200, "text/plain", "");   // Eigentlich beliebige Dateien
  }, handleFileUpload);

  server.begin();
  if (SerialDebug == 1)  Serial.println("HTTP Server gestarted");
  //while (1)server.handleClient();        // hier Programmende
}                                        // ausgang duch Reset vom INI wifimanager Webseite

boolean INI_Einlesen()
{
  String      line ;                              // Zeile von .ini Datei
  File        inifile ;                           // Datei

  if (!SPIFFS.begin(true))                        // SPIFFS Starten
  { // Wenn nicht da, formatieren
    if (SerialDebug == 1) Serial.println("SPIFFS Format nicht gelungen");
    return false;                                 // Wenn geht nich, dann fehlt die Partition
  }

  if ( !SPIFFS.exists("/setup.ini"))              // setup.ini  vorchanden?
  {
    if (SerialDebug == 1) Serial.println("setup.ini nicht da");
    File DataFile = SPIFFS.open("/setup.ini", "w");  // wenn nicht dann Anlegen
    DataFile.println("#INI neu angelegt bitte editieren");
    DataFile.println("SerialDebug = bitte eintragen");
    DataFile.println("NUM_LEDS = bitte eintragen");
    DataFile.println("BRIGHTNESS = bitte eintragen");
    DataFile.println("FRAMES_PER_SECOND = bitte eintragen");
    
    DataFile.close();
  }
  inifile = SPIFFS.open ( "/setup.ini", "r" ) ;        // WiFiManager INI-Datei offnen
  if (SerialDebug == 1) Serial.println("");
  if (SerialDebug == 1) Serial.println("Lese setup.ini");
  while (inifile.available())
  {
    line = inifile.readStringUntil ( '\n' ) ;         // Zeilenweise lesen
    analyzeZeile ( line.c_str() ) ;                   // und in der Funktion analyzeZeile bearbeiten
  }
  inifile.close() ;
  if (SerialDebug == 1)  Serial.println("");
  return true;                                        // WiFiManager INI -Datei erfolgreih gelesen
}



void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    if (SerialDebug == 1) Serial.print("Lade hoch: ");
    if (SerialDebug == 1) Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile)
      fsUploadFile.close();
    if (SerialDebug == 1) Serial.print("Hochgeladen: ");
    if (SerialDebug == 1) Serial.println(upload.totalSize);
  }
}


void analyzeZeile ( const char* str )
{
  char*  value ;                                 // Points to value after equalsign in command

  value = strstr ( str, "=" ) ;                  // See if command contains a "="
  if ( value )
  {
    *value = '\0' ;                              // Separate command from value
    value++ ;                                    // Points to value after "="
  }
  else
  {
    value = (char*) "0" ;                        // No value, assume zero
  }


  String             argument ;                       // Argument as string
  String             Wert ;                          // Value of an argument as a string
  int                inx ;                            // Index in string
  
  argument= str;
  if ( (inx = argument.indexOf ( "#" ) ) >= 0 )           // Comment line or partial comment?
  {
    argument.remove ( inx ) ;                              // Yes, remove
  }
  argument.trim() ;                                        // Remove spaces and CR
  if ( argument.length() == 0 )                       // Lege commandline (comment)?
  {
    return ;
  }
  argument.toLowerCase() ;                            // Force to lower case
  //value = chomp ( val ) ;                             // Get the specified value
  Wert=value ; 
  if ( (inx = Wert.indexOf ( "#" ) ) >= 0 )           // Comment line or partial comment?
  {
    Wert.remove ( inx ) ;                              // Yes, remove
  }
  Wert.trim() ;                                        // Remove spaces and CR
  
  if ( Wert.length() )
  {
    //if (SerialDebug == 1) Serial.println ( "Einstellung: " + argument + "  Wert: " + Wert ) ;
  }
  else
  {
    //if (SerialDebug == 1) Serial.println ( "Einstellung: " + argument + "  ohne Wert ") ;
  }  
DatenLaden(argument, Wert); 
}
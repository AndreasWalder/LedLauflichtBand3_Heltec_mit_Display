#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <WiFiClient.h>
#include <string.h>
#include <FS.h>
#include <WebServer.h>
#include "declare.h" //Includes und Variablen
WebServer server(80);
#include "INIwifimanager.h"
#include "LedControl.h" //Ansteuerung Led-Lauflicht
#include "SerialEvent.h" //Serielle Auswertung des Strings
#include "heltec.h"
#include "LiebherrLogo.h" //Liebherr Logo

#include <TaskScheduler.h>


// Callback methods prototypes
void t1Callback();
void t2Callback();
void t3Callback();
void t4Callback();

Task t1(1000, TASK_FOREVER, &t1Callback);
Task t2(100, TASK_FOREVER, &t2Callback);
Task t3(70000, TASK_FOREVER, &t3Callback);
Task t4(10, TASK_FOREVER, &t4Callback);

Scheduler runner;

void t1Callback() { 
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  
}

void t2Callback() { 
    serialEvent();   
    if (stringComplete)
     {  
         if (inputString == "Setup;") {
          Serial.println("Setup gestartet:");  
          Heltec.display->clear(); //OLET löschen    
          Heltec.display->drawString(25, 30, "Setup gestartet");
          Heltec.display->drawString(3, 44, "IP-Adresse: 192.168.4.1");
          Heltec.display->display(); //OLET anzeigen      
          FastLED.clear();  // clear all pixel data
          FastLED.show();
          t1.disable();
          t2.disable();
          t3.disable();       
          t4.enable();
          Einstellen();
          return;
         }
      
         BlinkenPosition = 0;
         BlinkenArrayWerte[10];
         FastLED.clear();  // clear all pixel data
         FastLED.show();       
         getSector(inputString);
         if (SerialDebug == 1) {
           Serial.println("Eingabe Ende");
         }     
         stringComplete = false;
     }
    if (BlinkenPosition > 0) {
      t3.enable();    
    }
    else 
    {
     t3.disable();   
    }
    
}

void t3Callback() {  
   int Zaehler = 0;
   
   for (int Wert=0; Wert<BlinkenPosition; Wert++){
    Zaehler++; 
    int Startpos = atoi(getValue(BlinkenArrayWerte[Zaehler], 0, '|').c_str());
    int Laenge = atoi(getValue(BlinkenArrayWerte[Zaehler], 1, '|').c_str());
    int ColorInt = atoi(getValue(BlinkenArrayWerte[Zaehler], 2, '|').c_str());
    int R = atoi(getValue(getValue(BlinkenArrayWerte[Zaehler], 2, '|'), 0, ',').c_str());
    int G = atoi(getValue(getValue(BlinkenArrayWerte[Zaehler], 2, '|'), 1, ',').c_str());
    int B = atoi(getValue(getValue(BlinkenArrayWerte[Zaehler], 2, '|'), 2, ',').c_str());
    int Blinken = atoi(getValue(BlinkenArrayWerte[Zaehler], 3, '|').c_str());

    BlinkenDurchgang[Zaehler] = !BlinkenDurchgang[Zaehler];

    for(int i = Startpos; i <= Laenge; i++)
    {  
      if (BlinkenDurchgang[Zaehler] == true) {
          leds[i] = CRGB(R*BRIGHTNESS,G*BRIGHTNESS,B*BRIGHTNESS);             
      }
      else
      {
        leds[i] = CRGB(0*BRIGHTNESS,0*BRIGHTNESS,0*BRIGHTNESS);    
      }
    }
  }
  FastLED.show();
}

void t4Callback() { //Setup - Mode:
   fadeToBlackBy( leds, NUM_LEDS, 40);
   int pos = beatsin16( 23, 0, NUM_LEDS-1 );
   leds[pos] += CHSV( gHue, 255, 192);
   FastLED.show();   
}


//******************************************************************************
  //  Diese Funktion wird beim lesen der Zeile der INI-Datei ausgeführt         *
  //******************************************************************************
void DatenLaden (String argument, String Wert)
{
    if (argument.indexOf ("serialdebug") >=0) {
      SerialDebug = Wert.toInt();
      Serial.print("SerialDebug: ");
      Serial.println(SerialDebug);
    }
    if (argument.indexOf ("frames_per_second") >= 0) {
      FRAMES_PER_SECOND = Wert.toInt();
      Serial.print("FRAMES_PER_SECOND: ");
      Serial.println(FRAMES_PER_SECOND);
    }
    if (argument.indexOf ("num_leds") >= 0) {
      NUM_LEDS = Wert.toInt();
      Serial.print("NUM_LEDS: ");
      Serial.println(NUM_LEDS); 
    }
    if (argument.indexOf ("brightness") >= 0) {
      BRIGHTNESS = Wert.toInt();
      Serial.print("BRIGHTNESS: ");
      Serial.println(BRIGHTNESS);  
    }
}


void setup()
{
  Serial.begin(57600);
  //Serial1.begin(57600,SERIAL_8N1,13,17);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Led Lauflicht Band3");
  //Serial1.println("Led Lauflicht Band3");

  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();  // clear all pixel data
  FastLED.show();       

  runner.init();
  Serial.println("Initialized scheduler");
  
  runner.addTask(t1);
  Serial.println("added t1");  
  runner.addTask(t2);
  Serial.println("added t2");
  runner.addTask(t3);
  Serial.println("added t3");
  runner.addTask(t4);
  Serial.println("added t4");
  
  t1.enable();
  Serial.println("Enabled t1");
  t2.enable();
  Serial.println("Enabled t2");
  t3.disable();
  Serial.println("Disable t3");
  t4.disable();
  Serial.println("Disable t4");



  // ----------------------OLET----------------------------------------------
   Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
   pinMode(25, OUTPUT); //OLET Beleuchtung
   digitalWrite(25,HIGH); //OLET Beleuchtung an
   Heltec.display->clear(); //OLET löschen
   Heltec.display->setFont(ArialMT_Plain_10);         //Schriftgröße 10
   Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT); //Schriftausrichtung
   Heltec.display->drawXbm(10, 8, Liebherr_Logo_width, Liebherr_Logo_height, Liebherr_Logo_bits);
   Heltec.display->drawString(3, 30, "Led Statusanzeige Band3");
   Heltec.display->drawString(40, 44, "(R) 2020");
   Heltec.display->display(); //OLET anzeigen
   delay(5000);
   //Heltec.display->clear(); //OLET löschen
   //Heltec.display->display(); //OLET anzeigen
   

   if (!INI_Einlesen()) {
      Serial.println("Setup gestartet:");  
      Heltec.display->clear(); //OLET löschen    
      Heltec.display->drawString(0, 15, "kein ini-File vorhanden");
      Heltec.display->drawString(25, 30, "Setup gestartet");
      Heltec.display->drawString(3, 44, "IP-Adresse: 192.168.4.1");
      Heltec.display->display(); //OLET anzeigen      
      FastLED.clear();  // clear all pixel data
      FastLED.show();
      Einstellen();     // INI -Datei einlesen, wenn geht nicht, starte INI WiFiManager
   }
 }

void loop() 
{
  runner.execute();
  server.handleClient();
}

void serialEvent() {
    while (Serial.available()) 
    {
       // get the new byte:
       char packet = Serial.read();
       char inChar = (char)packet;
       // add it to the inputString:
       inputString += inChar;
      // if the incoming character is a newline, set a flag
       // so the main loop can do something about it:
       if (packet==2)
       {
        inputString = "";
        stringComplete = false;
       }
       
       if (packet == 3) 
       {
         stringComplete = true;       
         inputString = inputString.substring(0, inputString.length()-1);
                      
         inputString = inputString + ";"; 
          if (SerialDebug == 1) {
           Serial.println("Serial Eingabe:");
           Serial.println(inputString);
          }       
         return;
       }
    }  
}

void getSector(String analyseString)
 {
    char string_array[analyseString.length()];
    analyseString.toCharArray(string_array, analyseString.length());
    char delimiter[] = {';'};
    char *ptr;
      
    ptr = strtok(string_array, delimiter);
    while(ptr != NULL) {
     if (SerialDebug == 1) {
       printf("Sektor gefunden: %s\n", ptr);
     }
     
      StringAuswerten(ptr);
      ptr = strtok(NULL, delimiter);
    }    
    return;
 }
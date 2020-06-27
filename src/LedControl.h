String getValue(String data, int index, char separator)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length()-1;
    for(int i=0; i<=maxIndex && found<=index; i++)
    {
      if(data.charAt(i)==separator || i==maxIndex)
      {
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
    }
    return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void StringAuswerten(String Sektor) 
{ 
  
  int Startpos = atoi(getValue(Sektor, 0, '|').c_str());
  int Laenge = atoi(getValue(Sektor, 1, '|').c_str());
  int ColorInt = atoi(getValue(Sektor, 2, '|').c_str());
  int R = atoi(getValue(getValue(Sektor, 2, '|'), 0, ',').c_str());
  int G = atoi(getValue(getValue(Sektor, 2, '|'), 1, ',').c_str());
  int B = atoi(getValue(getValue(Sektor, 2, '|'), 2, ',').c_str());
  int Blinken = atoi(getValue(Sektor, 3, '|').c_str());


  if (SerialDebug == 1) {
   Serial.println(Startpos);
   Serial.println(Laenge);
   Serial.println(R);
   Serial.println(G);
   Serial.println(B);
   Serial.println(Blinken);
  }
 

  if (Blinken == 1) {
    BlinkenPosition++;
    BlinkenArrayWerte[BlinkenPosition] = Sektor;
  }

  for(int i = Startpos; i <= Laenge; i++)
  {            
    leds[i] = CRGB(R*BRIGHTNESS,G*BRIGHTNESS,B*BRIGHTNESS);    
  }
  FastLED.show();  
 
  return;
}
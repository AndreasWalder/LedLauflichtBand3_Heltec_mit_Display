int SerialDebug = 1;    // 0 = ohne Debug ; 1 = mit Debug

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // the string is complete

#define DATA_PIN    21 // Data Pin Arduino
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
int NUM_LEDS = 144; // How many NeoPixels are attached to the Arduino?
CRGB leds[500];
int BRIGHTNESS = 255;
int FRAMES_PER_SECOND = 120;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

String BlinkenArrayWerte[10];
int BlinkenPosition;
bool BlinkenDurchgang[10];
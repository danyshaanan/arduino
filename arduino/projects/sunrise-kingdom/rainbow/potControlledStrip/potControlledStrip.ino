
#include <math.h>
#include <FastLED.h>


#define pi             3.14159265358979323846
#define BYTE           255         // FastLED channels are all bytes
#define PIN            11          // data pin for led strip
#define NUMPIXELS      60          // number of leds on led strip
#define fps            30          // frames per second
#define msPerPeriod    2000        // ms length of period
#define periodInPixels 45          // pixel length of period

#define POTPIN0        0           // potentiometer analog input pin
#define POTPIN1        1           // potentiometer analog input pin
#define POTPIN2        10          // potentiometer analog input pin



int msPerFrame = 1000 / fps;
float timesPerMs = 1 / float(msPerPeriod);
float huePerPixel = 1 / float(periodInPixels);

float temp;
int hue, value, saturation;

int t = 0;

CRGB leds[NUMPIXELS];


void setup() {
  FastLED.addLeds<WS2812, PIN>(leds, NUMPIXELS);
}


void loop() {
  t += map(analogRead(POTPIN0), 0, 1024, - msPerFrame, msPerFrame);
  t %= msPerPeriod;
  
  saturation = map(analogRead(POTPIN1), 0, 1024, 0, BYTE);
  hue = map(analogRead(POTPIN2), 0, 1024, 0, BYTE);

  for (int i = 0; i < NUMPIXELS; i++) {
    temp = timesPerMs * t - huePerPixel * i;
    value = BYTE * pow(sin(temp * pi * 2) / 2 + 0.5, 3);
    leds[i] = CHSV(hue, saturation, value);
  }

  FastLED.show();

  delay(msPerFrame);
}


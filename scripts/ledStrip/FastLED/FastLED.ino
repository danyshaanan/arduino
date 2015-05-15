
// Led strip animation using FastLED
// https://github.com/FastLED/FastLED

#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define BYTE           255         // FastLED channels are all bytes
#define PIN            6           // data pin for led strip
#define NUMPIXELS      70          // number of leds on led strip
#define fps            30          // frames per second
#define saturation     1           // saturation for hsv, [0-1]
#define value          0.4         // value for hsl, [0-1]
#define periodInMs     2000        // ms length of period
#define periodInPixels 30          // pixel length of period


float timesPerMs = 1 / float(periodInMs);
float huePerPixel = 1 / float(periodInPixels);
int msPerFrame = 1000 / fps;

int t = 0;
float hue;

CRGB leds[NUMPIXELS];

/////////////////////////////////////////////////////

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUMPIXELS);
}

void loop() {

  for (int i=0; i<NUMPIXELS; i++) {
    hue = timesPerMs * t - huePerPixel * i;
    leds[i] = CHSV(BYTE * hue, BYTE * saturation, BYTE * value);
  }
  
  FastLED.show();
  t = (t + msPerFrame) % periodInMs;
  delay(msPerFrame);
  
}

//////////////////////////////////////////////////////////

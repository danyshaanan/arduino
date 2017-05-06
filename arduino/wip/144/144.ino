
// Led strip animation using FastLED
// https://github.com/FastLED/FastLED

#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define BYTE           255         // FastLED channels are all bytes
#define PIN            11          // data pin for led strip
#define NUMPIXELS      144         // number of leds on led strip
#define fps            30          // frames per second
#define saturation     1.00        // saturation for hsv, [0-1]
#define value          0.25        // value for hsl, [0-1]
#define periodInMs     8000        // ms length of period
#define periodInPixels 72          // pixel length of period


int msPerFrame = 1000 / fps;
float timesPerMs = 1 / float(periodInMs);
float huePerPixel = 1 / float(periodInPixels);

int t;
float hue, temp;
CRGB leds[NUMPIXELS];

/////////////////////////////////////////////////////

void setup() {
  delay(5000);
  t = 0;
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMPIXELS);
}

void loop() {
  for (int i=0; i<NUMPIXELS; i++) {
    hue = timesPerMs * t - huePerPixel * i;
    leds[i] = CHSV(BYTE * hue, BYTE * saturation, BYTE * value);
  }

  FastLED.show();

  t = int(t + msPerFrame) % periodInMs;
  delay(msPerFrame);
}

//////////////////////////////////////////////////////////

// Led strip animation using FastLED
// https://github.com/FastLED/FastLED

#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define pi             3.14159265358979323846
#define BYTE           255         // FastLED channels are all bytes
#define PIN            6           // data pin for led strip (pwm)
#define NUMPIXELS      4          // number of leds on led strip
#define fps            50          // frames per second
#define saturation     1           // saturation for hsv, [0-1]
#define value          0.4         // value for hsl, [0-1]
#define periodInMs     10000       // ms length of period

#define programs       7

int msPerFrame = 1000 / fps;
float timesPerMs = 1 / float(periodInMs);

int t, program;
float hue, temp;
CRGB leds[NUMPIXELS];

/////////////////////////////////////////////////////

void setup() {
  t = 0;
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMPIXELS);
}

void loop() {

  hue = timesPerMs * t;

  for (int i=0; i<NUMPIXELS; i++) {
    leds[i] = CHSV(BYTE * hue, BYTE * saturation, BYTE * value);
  }
  
  FastLED.show();

  t = int(t + msPerFrame) % periodInMs;
  delay(msPerFrame);
}

//////////////////////////////////////////////////////////  

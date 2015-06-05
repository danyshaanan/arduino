
// Led strip animation using FastLED
// https://github.com/FastLED/FastLED

#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define pi             3.14159265358979323846
#define BYTE           255         // FastLED channels are all bytes
#define PIN            3           // data pin for led strip (pwm)
#define POTPIN1        0           // potentiometer analog input pin
#define POTPIN2        1           // potentiometer analog input pin
#define NUMPIXELS      20         // number of leds on led strip
#define fps            50          // frames per second
#define saturation     1           // saturation for hsv, [0-1]
#define value          0.4         // value for hsl, [0-1]
#define periodInMs     1000        // ms length of period
#define periodInPixels 15          // pixel length of period

#define programs       7

int msPerFrame = 1000 / fps;
float timesPerMs = 1 / float(periodInMs);
float huePerPixel = 1 / float(periodInPixels);

int t;
float hue;
CRGB leds[NUMPIXELS];

/////////////////////////////////////////////////////

void setup() {
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMPIXELS);
  t = 0;
  for (int i=0; i<NUMPIXELS; i++) {
    leds[i] = CHSV(0, 0, 100);
  }
  FastLED.show();
  delay(1000);
}

void loop() {

  for (int i=0; i<NUMPIXELS; i++) {
    hue = timesPerMs * t - huePerPixel * i * analogRead(POTPIN1)/1024;
    leds[i] = CHSV(BYTE * hue, BYTE * saturation, BYTE * value);
  }
  
  FastLED.show();

  t = int(float(t) + msPerFrame * analogRead(POTPIN2) / 1024) % periodInMs;
  delay(msPerFrame);
}

//////////////////////////////////////////////////////////  

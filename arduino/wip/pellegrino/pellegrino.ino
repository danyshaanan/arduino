#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define BYTE           255         // FastLED channels are all bytes
#define PIN            3           // data pin for led strip
#define NUMSENSORS     2           // number of sensors
#define NUMPIXELS      20          // number of leds on led strip
#define fps            30          // frames per second
#define saturation     1           // saturation for hsv, [0-1]
#define periodInMs     8000.       // ms length of period
#define sensitivity    100

int t, input, msPerFrame = 1000 / fps;
float hue, diff, value;
CRGB leds[NUMPIXELS];
int peak[NUMPIXELS] = { 0 };
int sensors[] = { 0, 1 };

/////////////////////////////////////////////////////

void setup() {
  Serial.begin (9600);
  t = 0;
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMPIXELS);
}

void loop() {
  hue = t / periodInMs;

  for (int i=0; i<NUMPIXELS; i++) leds[i] = CHSV(BYTE * hue, BYTE * saturation, BYTE * 0.2);
  for (int i=0; i<NUMSENSORS; i++) {
    if (analogRead(sensors[i]) > sensitivity) peak[i] = t;
    diff = (t - peak[i]) / 1000.0;
    value = peak[i] ? (diff < 1 ? diff : 1.0 / diff) : 0;
    if (value > .2) leds[i] = CHSV(BYTE * hue, BYTE * 0, BYTE * value);
  }

  FastLED.show();
  t += msPerFrame;
  delay(msPerFrame);
}

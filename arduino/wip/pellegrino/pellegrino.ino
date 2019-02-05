#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define BYTE           255         // FastLED channels are all bytes
#define DATAPIN        3           // data pin for led strip
#define NUMSENSORS     2           // number of sensors
#define NUMPIXELS      20          // number of WS2812 leds
#define fps            30          // frames per second
#define periodInMs     8000.       // ms length of period
#define sensitivity    50

int msPerFrame = 1000 / fps;
float s, hue, diff, value;
CRGB leds[NUMPIXELS];
float peak[NUMPIXELS] = { 0 };
int sensors[] = { 0, 1 };

/////////////////////////////////////////////////////

void setup() {
  Serial.begin (9600);
  s = 0.0;
  FastLED.addLeds<WS2812, DATAPIN>(leds, NUMPIXELS);
}

void loop() {
  hue = 1000.0 * s / periodInMs;

  for (int i = 0; i < NUMPIXELS; i++) leds[i] = CHSV(BYTE * hue, BYTE, BYTE * 0.2);
  for (int i = 0; i < NUMSENSORS; i++) {
    if (analogRead(sensors[i]) > sensitivity) peak[i] = s;
    diff = (s - peak[i]);
    value = peak[i] ? (diff < 1 ? diff : 1.0 / diff) : 0;
    if (value > .2) leds[i] = CHSV(BYTE * hue, 0, BYTE * value);
  }

  FastLED.show();
  delay(msPerFrame);
  s += msPerFrame / 1000.0;
}

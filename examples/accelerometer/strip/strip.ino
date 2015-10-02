// https://www.sparkfun.com/products/9269

#include <math.h>
#include <stdlib.h>
#include "FastLED.h" // https://github.com/FastLED/FastLED

#define PIXELSPIN      3 // data pin for led strip
#define VALUE          100 // out of 255
#define DIMENTIONS     3 // number of axis in 3d space and leds on led strip

CRGB leds[DIMENTIONS];
int axisSensorPins[] = { 1, 2, 3 };
int axisValue[] = { 0, 0, 0 };

void setup() {
  FastLED.addLeds<WS2811, PIXELSPIN>(leds, DIMENTIONS);
}

void loop() {
  for (int d=0; d<DIMENTIONS; d++) {
    axisValue[d] = (axisValue[d] + analogRead(axisSensorPins[d])) / 2;
    leds[d] = CRGB(axisValue[d] > 420 ? VALUE : 0, axisValue[d] < 300 ? VALUE : 0, 0);
  }
  FastLED.show();
  delay(30);
}


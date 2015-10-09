#include <math.h>
#include <stdlib.h>
#include "FastLED.h" // https://github.com/FastLED/FastLED

#define PIXELSPIN      11 // data pin for led strip
#define PIXELSNUM      5 // number of leds on led strip

CRGB leds[PIXELSNUM];

int h = 0;

void setup() {
  FastLED.addLeds<WS2812, PIXELSPIN>(leds, PIXELSNUM);
}

void loop() {
  for (int i=0; i<PIXELSNUM; i++) {
    leds[i] = CHSV((255 - h + i * 30) % 255, 255, 255 / 5);
  }
  h += 1;
  FastLED.show();
  delay(10);
}


#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define STRIPIN        11          // data pin for led strip
#define POTPIN1        A0          // potentiometer for value
#define POTPIN2        A10         // potentiometer for cycle length
#define POTPIN3        A11         // potentiometer for cycle time
#define NUMPIXELS      144         // number of leds on led strip

#define saturation     255         // saturation for hsv
#define msPerFrame     33          // miliseconds per frame
#define threshold      14          // threshold

float val, hps, hpp, p1, p2, p3, t1, t2, t3, hue = 0;
CRGB leds[NUMPIXELS];

/////////////////////////////////////////////////////

void setup() {
  FastLED.addLeds<NEOPIXEL, STRIPIN>(leds, NUMPIXELS);
}

void loop() {
  t1 = analogRead(POTPIN1);
  t2 = analogRead(POTPIN2);
  t3 = analogRead(POTPIN3);

  if (t1 < p1 - threshold || t1 > p1 + threshold) p1 = t1;
  if (t2 < p2 - threshold || t2 > p2 + threshold) p2 = t2;
  if (t3 < p3 - threshold || t3 > p3 + threshold) p3 = t3;

  val = p1 < 24 ? 0 : map(p1, 24, 1024, 64, 128);
  hpp = p2 < 24 ? 0 : map(p2, 24, 1024, 0, 255) / 24.0;
  hps = p3 < 24 ? 0 : map(p3, 24, 1024, 0, 255);

  hue += hps * msPerFrame / 1000.0;

  for (int i = 0; i < NUMPIXELS; i++) {
    leds[i] = CHSV(hue - hpp * i, saturation, val);
  }

  FastLED.show();
  delay(msPerFrame);
}

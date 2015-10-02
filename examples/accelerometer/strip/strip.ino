// https://www.sparkfun.com/products/9269

#include <math.h>
#include <stdlib.h>
#include "FastLED.h" // https://github.com/FastLED/FastLED

#define BYTE           255 // FastLED channels are all bytes
#define NUMPIXELS      4 // number of leds on led strip
#define PIXELSPIN      3 // data pin for led strip
#define VALUE          0.4
#define X              1 //analog pin
#define Y              2 //analog pin
#define Z              3 //analog pin
#define VERBOSE        1

CRGB leds[NUMPIXELS];

int x = 0;
int y = 0;
int z = 0;

int minVal = 320;
int maxVal = 400;

int asHue(int pin) {
  return constrain(map(analogRead(pin), minVal, maxVal, 0, 255), 0, 255);
}

int nintyTen(int p, int q) {
  return p * 0.7 + q * 0.3;
}

void setup() {
  FastLED.addLeds<WS2811, PIXELSPIN>(leds, NUMPIXELS);
  if (VERBOSE) Serial.begin(9600);
}

void loop() {
  x = nintyTen(x, asHue(X));
  y = nintyTen(y, asHue(Y));
  z = nintyTen(z, asHue(Z));

  leds[0] = CHSV(0, BYTE, x * VALUE);
  leds[1] = CHSV(0, BYTE, y * VALUE);
  leds[2] = CHSV(0, BYTE, z * VALUE);
  leds[3] = CRGB::Black;

  FastLED.show();
  delay(30);
}


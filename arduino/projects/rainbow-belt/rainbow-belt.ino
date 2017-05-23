#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define stripPin    11  // data output pin for WS2812 strip
#define potPin1     A0  // input pin for value potentiometer
#define potPin2     A10 // input pin for cycle length potentiometer
#define potPin3     A11 // input pin for cycle time potentiometer
#define numOfPixels 144
#define saturation  255
#define msPerFrame  33
#define threshold   32

CRGB leds[numOfPixels];
float value, huePerSecond, huePerPixel;
float input1, input2, input3, read1, read2, read3, hue = 0;

/////////////////////////////////////////////////////

void setup() {
  FastLED.addLeds<WS2812, stripPin>(leds, numOfPixels);
}

void loop() {
  read1 = analogRead(potPin1);
  read2 = analogRead(potPin2);
  read3 = analogRead(potPin3);

  if (abs(read1 - input1) > threshold) input1 = read1;
  if (abs(read2 - input2) > threshold) input2 = read2;
  if (abs(read3 - input3) > threshold) input3 = read3;

  value        = input1 < 24 ? 0 : map(input1, 24, 1024, 64, 128);
  huePerPixel  = input2 < 24 ? 0 : map(input2, 24, 1024, 0, 255) / 24.0;
  huePerSecond = input3 < 24 ? 0 : map(input3, 24, 1024, 0, 255);

  hue += huePerSecond * (msPerFrame / 1000.0);

  for (int pixel = 0; pixel < numOfPixels; pixel++) {
    leds[pixel] = CHSV(hue - huePerPixel * pixel, saturation, value);
  }

  FastLED.show();
  delay(msPerFrame);
}

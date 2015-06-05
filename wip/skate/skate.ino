/*

Ingredients:
* Latching Hall Effect Sensor - Melexis US1881
* RGB addressable LED strip

Built and tested on A-Star 32U4 Micro (Leonardo), with Arduino 1.6.3
https://www.pololu.com/product/3101

Connect the US1881 legs to Vcc, ground and A1.
Connect the LED strip legs to Vcc, ground, and 10.

*/

#include <math.h>
#include "FastLED.h"

#define StripPin       10
#define NUMPIXELS      4
#define BYTE           255
#define saturation     1           // saturation for hsv, [0-1]
#define value          0.4         // value for hsl, [0-1]
#define delayPerFrame  10

#define led            13 // build in led
#define analogInPin    A1
#define VERBOSE        1

const double tau = 2 * 3.14159265359;
const double periodsPedSecond = 0.133;

CRGB leds[NUMPIXELS];
double hue = 0.4;
double t = 0;

double x, seq;
boolean magnetState;

boolean getMagnetState(int pin) {
  return analogRead(pin) < 512;
}

////////////////////////////////////////////////////////

void setup() {
  FastLED.addLeds<NEOPIXEL, StripPin>(leds, NUMPIXELS);
  if (VERBOSE) Serial.begin(9600);
  pinMode(analogInPin, INPUT_PULLUP);
}


void loop() {
  magnetState = getMagnetState(analogInPin);
  if (VERBOSE) Serial.println(magnetState);
  analogWrite(led, magnetState ? 255 : 10);
  t += delayPerFrame * (magnetState ? 1 : -1);

  hue = 0.999 * hue + 0.001 * (magnetState ? 0 : 0.5); //This is not invariant to delayPerFrame!
  x = t / 1000 * tau * periodsPedSecond;

  for (int i=0; i<NUMPIXELS; i++) {
    seq = sin(x + i) / 2 + 0.5;
    leds[i] = CHSV(BYTE * hue, BYTE * saturation, BYTE * seq * value);
  }
  FastLED.show();

  delay(delayPerFrame);
}

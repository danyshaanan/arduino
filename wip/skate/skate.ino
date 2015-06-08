/*

Ingredients:
* Latching Hall Effect Sensor - Melexis US1881
* RGB addressable LED strip (fastled.io compatible)

Built and tested on A-Star 32U4 Micro (Leonardo), with Arduino 1.6.3
https://www.pololu.com/product/3101

Connect the US1881 legs to Vcc, ground and digital 11.
Connect the LED strip legs to Vcc, ground, and 10.

The magnet should move in the direction of its magnetic axis,
so that the sensor will sense its two poles right before and right after it passes it.

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
#define analogInPin    11
#define VERBOSE        1

const double tau = 2 * 3.14159265359;
const double periodsPedSecond = 1;

CRGB leds[NUMPIXELS];
double hue = 0;
double t = 0;

double x, seq;
boolean magnetState = false;
boolean prevMagnetState = false;
int positiveCount = 0;
int negativeCount = 0;
int msSinceLastTick = 0;
double mm = 200;
double speed = 0;

boolean getMagnetState(int pin) {
  return digitalRead(pin);
}

////////////////////////////////////////////////////////

void setup() {
  FastLED.addLeds<NEOPIXEL, StripPin>(leds, NUMPIXELS);
  if (VERBOSE) Serial.begin(9600);
  pinMode(analogInPin, INPUT_PULLUP);
}

void loop() {
  magnetState = getMagnetState(analogInPin);
  analogWrite(led, magnetState ? 255 : 10);

  if (magnetState && !prevMagnetState) {
    speed = (positiveCount > negativeCount ? 1 : -1) * mm/float(msSinceLastTick);
    if (VERBOSE) Serial.println(speed);

    msSinceLastTick = 0;
    positiveCount = 0;
    negativeCount = 0;
  } else {
    double maxAbsSpeed = mm/float(msSinceLastTick);
    if (speed > 1.3 * maxAbsSpeed) speed = maxAbsSpeed;
    if (-speed > 1.3 * maxAbsSpeed) speed = -maxAbsSpeed;
  }

  if (VERBOSE) Serial.println(speed);

  if (magnetState) positiveCount++;
  else             negativeCount++;

  t += delayPerFrame * speed;
  msSinceLastTick += delayPerFrame;

  x = t / 1000 * tau * periodsPedSecond;

  for (int i=0; i<NUMPIXELS; i++) {
    seq = sin(x + 0.25 * i) / 2 + 0.5;
    leds[i] = CHSV(BYTE * seq, BYTE * saturation, BYTE * value);
  }
  FastLED.show();

  prevMagnetState = magnetState;
  delay(delayPerFrame);
}

/*********************************************************************
 *                                                                  *
 *                                                                  *
 *                  Two Potentiometers One WS2812                   *
 *                                                                  *
 *                                                                  *
 *         /``````\                              ___                *
 *        | WS2812 |                            /   \               *
 *        |________|                           / pot \              *
 *         | |  | |                            |_____|              *
 *         | |  | |__                          |  |  |              *
 *         | |  |   _|_________________________|__|__|__            *
 *         | |  |  | 0                         7  8  9 |            *
 *         | |  |  |                                   |___         *
 *        /  |  |  |                                     U |        *
 *           |  |  |         A-Star 32U4 Micro           S |        *
 *           |  |  |                                    _B_|        *
 *           |  |  |                                   |            *
 *           |  |  |     G   V                10 A1 A0 |            *
 *           |  |   `````|```|`````````````````|``|``|``            *
 *           |  |________|___|___|`````````|   |__|__|              *
 *           |___________|_______|   CAP   |   \ pot /              *
 *                               |_________|    \___/               *
 *                                                                  *
 *                                                                  *
 *                                                                  *
 ********************************************************************/

#include "FastLED.h" // https://github.com/FastLED/FastLED

//////////////////////////////////////////////////////////////////

#define PIXELSPIN      0 // WS2812 data pin
#define FPS           60 // frames per second

CRGB leds[1];
float t = 0;
float secondsPerFrame = 1. / FPS;
int pots[][3] = { { 7, 8, 9 }, { A0, A1, 10 } };

//////////////////////////////////////////////////////////////////

float read(int pot[]) {
  return analogRead(pot[1]) / 1024.0;
}

void hsv(int h, int s, int v) {
  leds[0] = CHSV(h % 256, s % 256, v % 256);
}

void setPot(int pot[]) {
  pinMode(pot[0], OUTPUT);
  digitalWrite(pot[0], HIGH);
  pinMode(pot[2], OUTPUT);
  digitalWrite(pot[2], LOW);
}

//////////////////////////////////////////////////////////////////

void setup() {
  for (int i = 0; i < 2; i++) setPot(pots[i]);
  FastLED.addLeds<WS2812, PIXELSPIN>(leds, 1);
}

void loop() {
  t += secondsPerFrame * read(pots[0]);
  hsv(256 * t, 255, 256 * read(pots[1]));
  FastLED.show();
  delay(1000 * secondsPerFrame);
}


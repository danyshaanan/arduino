#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define BYTE           255         // FastLED channels are all bytes
#define DATAPIN        2           // data pin for led strip
#define NUMPIXELS      12          // number of WS2812 leds
#define fps            60          // frames per second
#define periodInMs     8000.       // ms length of period
#define sensitivity    30          // sensor threashold out of 1024
#define secondsAction  1.0         // seconds for initial led response
#define V              255         // light intensity out of 255
#define S              255         // saturation out of 255

int msPerFrame = 1000 / fps;
float now, H, diff, value;
CRGB leds[NUMPIXELS];
float peaks[NUMPIXELS];
int reads[NUMPIXELS];
int threshold[NUMPIXELS] = { 40, 20, 20, 20, 25, 25, 30, 25, 30, 25, 35, 35 };
//                            0   1   2   3   4   5   6   7   8   9  10  11
int lastWave = 0;
int lastPeak = 0;

/////////////////////////////////////////////////////

void setup() {
  now = 0.0;
  Serial.begin (9600);
  FastLED.addLeds<WS2812, DATAPIN>(leds, NUMPIXELS);
}

void loop() {
  H = BYTE * 1000.0 * now / periodInMs;

  //identify wave:
  int peaksLately = 0;
  for (int i = 0; i < NUMPIXELS; i++) {
    if (now - 6 < peaks[i]) peaksLately++;
  }
  if (lastWave < now - 7 && lastPeak < now - 1 && peaksLately >= 5) {
    Serial.println("wave identified");
    lastWave = now;
  }

  //read:
  for (int i = 0; i < NUMPIXELS; i++) {
    reads[i] = analogRead(i + 1);
    if (reads[i] > threshold[i]) {
      Serial.println(i);
      peaks[i] = now;
      lastPeak = now;
    }
  }

  //write all:
  for (int i = 0; i < NUMPIXELS; i++) {
    leds[i] = CHSV(1 * H, S, .2 * V);
  }

  //write hits:
  for (int i = 0; i < NUMPIXELS; i++) {
    if (now - peaks[i] < secondsAction) leds[i] = CHSV(8 * H, S, 1. * V);
  }

  //
  
  FastLED.show();
  delay(msPerFrame);
  now += msPerFrame / 1000.0;
}

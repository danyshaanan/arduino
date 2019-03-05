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
float now;
int H, maxHit, maxPin, hit;
CRGB leds[NUMPIXELS];
float peaks[NUMPIXELS];
int wave[NUMPIXELS];
int temp[NUMPIXELS];
int alll[NUMPIXELS];
int none[NUMPIXELS];
int threshold[NUMPIXELS] = { 40, 20, 20, 20, 25, 25, 30, 25, 30, 25, 35, 35 };
//                            0   1   2   3   4   5   6   7   8   9  10  11
int lastWave = 0;
int lastPeak = 0;

/////////////////////////////////////////////////////

void wait(int ms) {
  delay(ms);
  now += ms / 1000.0;
}
void writeTo(int wave[], int h = 0, int s = 0, int v = 0, int ms = 0) {
  for (int i = 0; i < NUMPIXELS; i++) if (wave[i]) leds[i] = CHSV(h, s, v);
  FastLED.show();
  wait(ms);
}
int count(int arr[]) {
  int c = 0;
  for (int i = 0; i < NUMPIXELS; i++) if (arr[i]) c++;
  return c;
}
/////////////////////////////////////////////////////

void setup() {
  for (int i = 0; i < NUMPIXELS; i++) {
    none[i] = 0;
    alll[i] = 1;
  }
  now = 0.0;
  Serial.begin (9600);
  FastLED.addLeds<WS2812, DATAPIN>(leds, NUMPIXELS);
}

void loop() {
  H = BYTE * 1000.0 * now / periodInMs;

  //identify wave:
  for (int i = 0; i < NUMPIXELS; i++) wave[i] = now - 6 < peaks[i];
  if (lastWave < now - 5 && lastPeak < now - 1 && count(wave) >= 4) {
    Serial.println("wave identified");
    lastWave = now;
    writeTo(alll, 0, 0, 0);
    for (int j = 0; j < 3; j++) {
      writeTo(wave, 0, 0, V, 200);
      writeTo(alll, 0, 0, 0, 200);
    }
  }

  //read:
  maxHit = 0;
  for (int i = 0; i < NUMPIXELS; i++) {
    hit = analogRead(i + 1) - threshold[i];
    if (hit > maxHit) {
      maxHit = hit;
      maxPin = i;
    }
  }
  if (maxHit) {
    Serial.println(maxPin);
    peaks[maxPin] = now;
    lastPeak = now;
  }

  //write all:
  for (int i = 0; i < NUMPIXELS; i++) temp[i] = now - peaks[i] < secondsAction;
  writeTo(alll, 1 * H, S, 0.2 * V);
  writeTo(temp, 8 * H, S, 1.0 * V);
  wait(msPerFrame);
}

#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define BYTE           255         // FastLED channels are all bytes
#define DATAPIN        2           // data pin for led strip
#define NUMPIXELS      12          // number of WS2812 leds
#define fps            60          // frames per second
#define secondsAction  1.0         // seconds for initial led response
#define V              255         // light intensity out of 255
#define S              255         // saturation out of 255
#define NUMPROGRAMS    3
#define DISABLEWAVE    false

int msPerFrame = 1000 / fps;
float now, valueHitReducer;
int H, maxHit, maxPin, hit;
CRGB leds[NUMPIXELS];
float peaks[NUMPIXELS];
int temp[NUMPIXELS];
int wave[NUMPIXELS];
int hits[NUMPIXELS];
int alll[NUMPIXELS];
int none[NUMPIXELS];
int threshold[NUMPIXELS] = { 40, 20, 20, 20, 25, 25, 30, 25, 30, 25, 35, 35 };
//                            1   2   3   4   5   6   7   8   9  10  11  12
int lastWave = 0;
int lastPeak = 0;
int program = 0;

/////////////////////////////////////////////////////

void wait(int ms) {
  delay(ms);
  now += ms / 1000.0;
}
void writeTo(int wave[], int h = 0, int s = 0, int v = 0, int ms = 0) {
  for (int i = 0; i < NUMPIXELS; i++) if (wave[i]) leds[i] = CHSV(h, s, v);
  if (ms == 0) return;
  FastLED.show();
  wait(ms);
}
int count(int arr[]) {
  int c = 0;
  for (int i = 0; i < NUMPIXELS; i++) if (arr[i]) c++;
  return c;
}

/////////////////////////////////////////////////////

void runProgram(int p) {
  H = BYTE * now;
  valueHitReducer = count(hits) ? 0.5 : 1.0;

  if (p == 0) {
    writeTo(alll, 1.0 * H, S, valueHitReducer * V);
  }
  if (p == 1) {
    writeTo(alll, 0.1 * H, S, 0.5 * V);
    int bottles = 12;
    int bottle = abs(((int)(8 * (now + 0 * sin(3.2 * now))) % (2 * bottles - 2)) - bottles + 2);
    leds[bottle] = CHSV(0, 0, valueHitReducer * V);
  }
  if (p == 2) {
    writeTo(alll, 0, 0, 0);
    int bottles = count(wave);
    int bottle = abs(((int)(4 * (now + sin(3.2 * now))) % (2 * bottles - 2)) - bottles + 2) + 1;
    int bottleCounter = 0;
    int j;
    for (j = 0; j < NUMPIXELS; j++) {
      if (wave[j]) bottleCounter++;
      if (bottleCounter == bottle) break;
    }
    leds[j] = CHSV(0, S, V);
  }
  
}

/////////////////////////////////////////////////////

void setup() {
  now = 0.0;
  for (int i = 0; i < NUMPIXELS; i++) {
    none[i] = 0;
    alll[i] = 1;
    peaks[i] = -60;
  }
  FastLED.addLeds<WS2812, DATAPIN>(leds, NUMPIXELS);
  Serial.begin (9600);
  delay(5000);
}

void loop() {
  //identify wave:
  for (int i = 0; i < NUMPIXELS; i++) temp[i] = now - 6 < peaks[i];
  if (!DISABLEWAVE && lastWave < now - 6 && lastPeak < now - 1 && count(temp) >= 4) {
    Serial.println("wave identified");
    for (int i = 0; i < NUMPIXELS; i++) wave[i] = temp[i];
    lastWave = now;
    program = (program + 1) % NUMPROGRAMS;
    for (int j = 0; j < 5; j++) {
      writeTo(alll, 0, 0, 0, 200);
      writeTo(wave, 0, 0, V, 200);
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
  if (maxHit > 20) {
    Serial.println(maxPin + 1);
    peaks[maxPin] = now;
    lastPeak = now;
  }
  for (int i = 0; i < NUMPIXELS; i++) hits[i] = now - peaks[i] < secondsAction;

  //write all (programs):
  runProgram(program);

  //write active and wait for frame:
  writeTo(hits, 0, 0, V, msPerFrame);
}

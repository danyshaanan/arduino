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
#define NUMPROGRAMS    7
#define DISABLEWAVE    false
#define autoSwitchSecs 30

int msPerFrame = 1000 / fps;
float now, valueHitReducer;
int H, maxHit, maxPin, hit, red;
CRGB leds[NUMPIXELS];
float peaks[NUMPIXELS];
int temp[NUMPIXELS];
int wave[NUMPIXELS] = { 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0 };
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
  valueHitReducer = count(hits) ? 0.1 : 1.0;
  red = 255.0 * 5.0 / 12.0;
//  if (p == 0) for (int i = 0; i < NUMPIXELS; i++) leds[i] = CHSV(255.0 / 12.0 * i, S, V); // color test

  if (p == 0) { // sunrise
    for (int i = 0; i < NUMPIXELS; i++) leds[i] = CHSV(255.0 / 12.0 * (i + 4 * now), S, valueHitReducer * V);
  }

  if (p == 1) { // path ping ping
    writeTo(alll, red, S, valueHitReducer * V);
    int bottles = count(wave);
    int bottle = abs(((int)(4 * (now + sin(3.2 * now))) % (2 * bottles - 2)) - bottles + 2) + 1;
    int bottleCounter = 0;
    int j;
    for (j = 0; j < NUMPIXELS; j++) {
      if (wave[j]) bottleCounter++;
      if (bottleCounter == bottle) break;
    }
    leds[j] = CHSV(0, S, valueHitReducer * V);
  }

  if (p == 2) { // upwards flood
    writeTo(alll, 0.1 * H, S, 0.5 * valueHitReducer * V);
    int bottle = (int)(8 * now) % 24;
    for (int i = bottle - 12; i <= bottle; i++) if (0 <= i && i < 12) leds[i] = CHSV(0, 0, valueHitReducer * V);
  }

  if (p == 3) { // yellow blue lights
    int yellow = 256.0 * 4.0 / 12.0;
    int purple = 256.0 * 10.0 / 12.0;
    int perm[NUMPIXELS] = { 2, 5, 11, 1, 9, 7, 4, 0, 10, 3, 8, 6 };
    writeTo(alll, yellow, S, valueHitReducer * V);
    int bottle = (int)(12 * now) % 24;
    for (int i = bottle - 12; i <= bottle; i++) if (0 <= i && i < 12) leds[perm[i]] = CHSV(purple, S, valueHitReducer * V);
  }
  
  if (p == 4) { // clock
    int blue = 255.0 * 11.0 / 12.0;
    writeTo(alll, red, S, valueHitReducer * V);
    int inner[4] = { 3, 6, 8, 5 };
    int outer[8] = { 0, 1, 2, 7, 11, 10, 9, 4 };
    leds[inner[(int)(3200 + 128 * 1 * cos(.1 * now) + 0) % 4]] = CHSV(blue, S, 1.00 * valueHitReducer * V);
    leds[outer[(int)(3200 + 128 * 2 * cos(.1 * now) + 0) % 8]] = CHSV(blue, S, 0.40 * valueHitReducer * V);
    leds[outer[(int)(3200 + 128 * 2 * cos(.1 * now) + 1) % 8]] = CHSV(blue, S, 1.00 * valueHitReducer * V);
  }

  if (p == 5) {
    writeTo(alll, 1.0 * H, S, valueHitReducer * V); // uniform changing hue
  }
  
  if (p == 6) { // all ping pong
    writeTo(alll, red, S, valueHitReducer * V);
    int bottles = 12;
    int bottle = abs(((int)(8 * now) % (2 * bottles - 2)) - bottles + 2);
    leds[bottle] = CHSV(0, 0, valueHitReducer * V);
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
  delay(1000);
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
  if (lastWave + 6 < now - autoSwitchSecs) {
    lastWave = now - 6;
    program = (program + 1) % NUMPROGRAMS;    
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

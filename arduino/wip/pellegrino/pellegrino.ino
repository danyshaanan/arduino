#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define BYTE           255         // FastLED channels are all bytes
#define DATAPIN        2           // data pin for led strip
#define NUMPIXELS      12          // number of WS2812 leds
#define fps            100         // frames per second
#define periodInMs     8000.       // ms length of period
#define sensitivity    30          // sensor threashold out of 1024
#define secondsAction  1.0         // seconds for initial led response
#define v              1.0         // light intensity out of 1

int msPerFrame = 1000 / fps;
float s, hue, diff, value;
CRGB leds[NUMPIXELS];
float peak[NUMPIXELS];


/////////////////////////////////////////////////////

void setup() {
  Serial.begin (9600);
  s = 0.0;
  FastLED.addLeds<WS2812, DATAPIN>(leds, NUMPIXELS);
}

void loop() {
  hue = 1000.0 * s / periodInMs;

  for (int i = 0; i < NUMPIXELS; i++) {
    if (analogRead(i + 1) > sensitivity) peak[i] = s;
    diff = s - peak[i];
    if (diff < secondsAction) leds[i] = CHSV(BYTE * hue, 0, (1.0 - diff / secondsAction) * BYTE * v);
    else                      leds[i] = CHSV(BYTE * hue, BYTE, BYTE * .5 * v);
    Serial.print(analogRead(i + 1) / 100);
    Serial.print(" ");
  }
  Serial.println("");
  
  FastLED.show();
  delay(msPerFrame);
  s += msPerFrame / 1000.0;
}

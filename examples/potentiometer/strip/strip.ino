
// Led strip animation using FastLED
// https://github.com/FastLED/FastLED

#include "FastLED.h"
#include <stdlib.h>

#define BYTE           255         // FastLED channels are all bytes
#define PIN            6           // data pin for led strip
#define SENSORPIN      7           // sensor analog input pin
#define NUMPIXELS      20          // number of leds on led strip
#define fps            30          // frames per second

#define maxSpeed       20

int msPerFrame = 1000 / fps;

int t;
CRGB leds[NUMPIXELS];

/////////////////////////////////////////////////////

void setup() {
  t = 0;
  FastLED.addLeds<WS2812, PIN>(leds, NUMPIXELS);
}

void loop() {
  for (int i=0; i<NUMPIXELS; i++) {
    leds[i] = CRGB::Black;
  }
  leds[int(t / 1000) % NUMPIXELS] = CRGB::Red;
  FastLED.show();

  t += msPerFrame * map(analogRead(SENSORPIN), 0, 1024, 1, maxSpeed);
  delay(msPerFrame);
}

//////////////////////////////////////////////////////////

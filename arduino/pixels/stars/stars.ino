#include <math.h>
#include <FastLED.h>

#define BYTE            255         // FastLED channels are all bytes
#define PIN             11          // data pin for led strip
#define NUMPIXELS       6          // number of leds on led strip
#define fps             60          // frames per second

int msPerFrame = 1000 / fps;


int t = 0;
int v;

CRGB leds[NUMPIXELS];
float ledsValues[NUMPIXELS];
float ledsDirections[NUMPIXELS];


void grayscaleLed(int i, int v) {
  leds[i] = CRGB(v, v, v);
}

void setup() {
  delay(3000);
  FastLED.addLeds<WS2812, PIN>(leds, NUMPIXELS);
  for (int i = 0; i < NUMPIXELS; i++) {
    ledsValues[i] = 0;
    ledsDirections[i] = 0;
  }
}

void loop() {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (ledsDirections[i]) {
      ledsValues[i] += ledsDirections[i];
      if (ledsValues[i] > 1) {
        ledsValues[i] = 1;
        ledsDirections[i] = -0.01;
      }
      if (ledsValues[i] < 0) {
        ledsValues[i] = 0;
        ledsDirections[i] = 0;
      }
    } else if (rand() % 1000 < 1) {
      ledsDirections[i] = 0.001;
    }

    grayscaleLed(i, BYTE / 2 * ledsValues[i]);
  }
  FastLED.show();

  delay(msPerFrame);
  t += msPerFrame;
}


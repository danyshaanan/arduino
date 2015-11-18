
// Led strip animation using FastLED
// https://github.com/FastLED/FastLED

#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define pi             3.14159265358979323846
#define BYTE           255         // FastLED channels are all bytes
#define PIN            11          // data pin for led strip
#define NUMPIXELS      22          // number of leds on led strip
#define fps            30          // frames per second
#define saturation     1           // saturation for hsv, [0-1]
#define value          1.0         // value for hsl, [0-1]

const double tauPerMinute = 2 * M_PI / 60000;
const double hb = 40;

double t = 0;

int msPerFrame = 1000 / fps;

float hue, temp;

CRGB leds[NUMPIXELS];

/////////////////////////////////////////////////////

void hsv(int i, float h, float s, float v) {
  leds[i] = CHSV(BYTE * h, BYTE * saturation * s, BYTE * value * v);
}

void rgb(int i, float r, float g, float b) {
  leds[i] = CRGB(BYTE * value * r, BYTE * value * g, BYTE * value * b);
}

/////////////////////////////////////////////////////


void setup() {
  FastLED.addLeds<WS2812, PIN>(leds, NUMPIXELS);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  temp = pow(0.01 * map(100 * sin(t), -100, 100, 60, 90), 4);

  for (int i=0; i<NUMPIXELS; i++) {
    hsv(i, 0, 1, temp);
  }

  FastLED.show();
  t += msPerFrame * hb * tauPerMinute;
  delay(msPerFrame);
}


#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define piezPin        0
#define pi             3.14159265358979323846
#define BYTE           255         // FastLED channels are all bytes
#define PIN             3          // data pin for led strip
#define NUMPIXELS      1           // number of leds on led strip
#define fps            30          // frames per second
#define saturation     1           // saturation for hsv, [0-1]
#define periodInMs     2000        // ms length of period



int msPerFrame = 1000 / fps;
float timesPerMs = 1 / float(periodInMs);

int t, program;
float hue, temp;
CRGB leds[NUMPIXELS];


float value = 0;
/////////////////////////////////////////////////////

void setup() {
  Serial.begin (9600);
  t = 0;
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMPIXELS);
}

void loop() {
  int piez = analogRead(piezPin);
  Serial.println(piez);

  if (piez > 100) value = 1.0;
  else value *= 0.9;
    
  for (int i=0; i<NUMPIXELS; i++) {
    hue = timesPerMs * t;
    leds[i] = CHSV(BYTE * hue, BYTE * saturation, BYTE * value);
  }

  FastLED.show();

  t = int(t + msPerFrame) % periodInMs;
  delay(msPerFrame);
}


// Led strip animation using FastLED
// https://github.com/FastLED/FastLED

#include "FastLED.h"
#include <stdlib.h>
#include <math.h>

#define pi             3.14159265358979323846
#define BYTE           255         // FastLED channels are all bytes
#define PIN            11          // data pin for led strip
#define POTPIN         0           // potentiometer analog input pin
#define NUMPIXELS      70          // number of leds on led strip
#define fps            30          // frames per second
#define saturation     1           // saturation for hsv, [0-1]
#define value          1.0         // value for hsl, [0-1]
#define periodInMs     2000        // ms length of period
#define periodInPixels 45          // pixel length of period

#define programs       7

int msPerFrame = 1000 / fps;
float timesPerMs = 1 / float(periodInMs);
float huePerPixel = 1 / float(periodInPixels);

int t, program;
float hue, temp;
CRGB leds[NUMPIXELS];

/////////////////////////////////////////////////////

void setup() {
  t = 0;
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMPIXELS);
}

void loop() {

  program = int(programs * analogRead(POTPIN) / 1024);

  for (int i=0; i<NUMPIXELS; i++) {

    hue = timesPerMs * t - huePerPixel * i;
  
    if (program == 0) { // "off"
      leds[i] = CRGB::Black;
      if (i == 0) leds[i] = CHSV(131, BYTE * 0.77, BYTE * 0.20);
    } else if (program == 1) { // fast rainbow
      leds[i] = CHSV(BYTE * hue, BYTE * saturation, BYTE * value);
    } else if (program == 2) { // orange
      temp = pow(sin(hue * pi * 2) / 2 + 0.5, 3);
      leds[i] = CRGB(BYTE * value * temp, BYTE * value * temp * 0.5, BYTE * value * 0);
    } else if (program == 3) { // blue
      hue = - timesPerMs * t - huePerPixel * i;
      temp = pow(sin(hue * pi * 2) / 2 + 0.5, 3);
      leds[i] = CRGB(BYTE * value * 0, BYTE * value * 0, BYTE * value * temp);
    } else if (program == 4) { // pink
      temp = pow(sin(hue * pi * 2) / 2 + 0.5, 3);
      leds[i] = CHSV(241, BYTE * 0.77, BYTE * temp);
    } else if (program == 5) { // ???
      temp = pow(sin(hue * pi * 2) / 2 + 0.5, 3);
      leds[i] = CHSV(BYTE * hue, BYTE * 0.77, BYTE * temp);
    } else if (program == 6) { // "off"
      leds[i] = CRGB::Black;
      if (i == 0) leds[i] = CHSV(241, BYTE * 0.77, BYTE * 0.20);
    } else { // error!
      leds[i] = CRGB::Black;
    }
  }
  
  FastLED.show();

  t = int(t + msPerFrame) % periodInMs;
  delay(msPerFrame);
}

//////////////////////////////////////////////////////////  

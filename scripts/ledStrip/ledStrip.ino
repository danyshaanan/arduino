
// Led strip animation using Adafruit_NeoPixel
// Follow install instructions from:
// https://github.com/adafruit/Adafruit_NeoPixel

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdlib.h>
#include <math.h>

#define maxChannel     255         // max color channel
#define PIN            6           // data pin for led strip
#define NUMPIXELS      70          // number of leds on led strip
#define fps            30          // frames per second
#define saturation     1           // saturation for hsl, [0-1]
#define value          0.2         // value for hsl, [0-1]
#define periodInMs     2000        // ms length of period
#define periodInPixels 30          // pixel length of period


float timesPerMs = 1 / float(periodInMs);
float huePerPixel = 1 / float(periodInPixels);
int msPerFrame = 1000 / fps;

int hue, t = 0;
float r, g, b;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/////////////////////////////////////////////////////

void setup() {
  pixels.begin();
}


void loop() {

  for (int i=0; i<NUMPIXELS; i++){
    hue = int(360 * (1 + timesPerMs * t - huePerPixel * i)) % 360; 
    HSVtoRGB(&r, &g, &b, hue, saturation, value);
    pixels.setPixelColor(i, pixels.Color(maxChannel * r, maxChannel * g, maxChannel * b));
  }
  
  pixels.show();
  t = (t + msPerFrame) % periodInMs;
  delay(msPerFrame);
}

//////////////////////////////////////////////////////////

// hue is in [0,360)
// s, v, r, g, b are in [0,1]
// if s == 0, then h = -1 (undefined)

void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v) {
  int i;
  float f, p, q, t;
  if (s == 0) {
    *r = *g = *b = v;
    return;
  }
  h /= 60;
  i = floor(h);
  f = h - i;
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch(i) {
    case 0:
      *r = v;
      *g = t;
      *b = p;
      break;
    case 1:
      *r = q;
      *g = v;
      *b = p;
      break;
    case 2:
      *r = p;
      *g = v;
      *b = t;
      break;
    case 3:
      *r = p;
      *g = q;
      *b = v;
      break;
    case 4:
      *r = t;
      *g = p;
      *b = v;
      break;
    default:  // case 5:
      *r = v;
      *g = p;
      *b = q;
      break;
  }
}

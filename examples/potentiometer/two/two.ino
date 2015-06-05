#include <math.h>

#define pi             3.14159265358979323846
#define BYTE           255         // FastLED channels are all bytes
#define fps            30          // frames per second
const double tauPerMinute = 2 * M_PI / 60000;

double hb = 40;
double t = 0;
int msPerFrame = 1000 / fps;

///////////////////////

#define POTPIN1        0           // potentiometer analog input pin
#define POTPIN2        1           // potentiometer analog input pin
#define LEDPIN         LED_BUILTIN
#define VERBOSE        0

int tick = 10;

void setup() {
  pinMode(LEDPIN, OUTPUT);
  if (VERBOSE) Serial.begin(9600);
}

float f(float t, float p) {
  return 255 * pow(sin(t) / 2 + 0.5, p);
}

void loop() {
  analogWrite(LEDPIN, f(t, map(analogRead(POTPIN2), 0, 1023, 1, 12)));

  hb = map(analogRead(POTPIN1), 0, 1023, 20, 140);
  t += msPerFrame * hb * tauPerMinute;
  delay(msPerFrame);
}

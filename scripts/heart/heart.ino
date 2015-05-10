
#include <math.h>

const int ledPin = 5;     // this <-> 330 resistor <-> led <-> ground
const int potPin = 0;     // this <-> center pot terminal. VCC and ground to other two terminals

const double cycleTime = 18;
const int minPulse = 20;
const int maxPulse = 140;
const double tauPerMinute = 2 * M_PI / 60000;
const int verbose = 1;

double hb = 20;
double t = 0;

///////////////////////////////////////////////////////////////////////

double linear(double value, double x1, double x2, double y1, double y2) {
  return (value - x1)*(y2 - y1)/(x2 - x1) + y1;
}

void setup() {
  if (verbose == 1) Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  hb = linear(analogRead(potPin), 0, 1023, minPulse, maxPulse);
  if (verbose) Serial.println(hb);
  
  analogWrite(ledPin, 255 * pow(sin(t)/2+0.5, 4));
  
  t += cycleTime * hb * tauPerMinute;
  delay(cycleTime);
}


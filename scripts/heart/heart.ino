
#include <math.h>

const int pButtonPin = 3; // v <->  plus button <-> this <-> 10k resistor <-> groud
const int mButtonPin = 4; // v <-> minus button <-> this <-> 10k resistor <-> groud
const int ledPin = 5;     // this <-> 330 resistor <-> led <-> ground

const double cycleTime = 18;
const double hbChangePerLoop = cycleTime * 40 / 1000;
const int minPulse = 20;
const int maxPulse = 140;
const double tauPerMinute = 2 * M_PI / 60000;
const int verbose = 1;

double hb = 20;
double t = 0;

///////////////////////////////////////////////////////////////////////

void setup() {
  if (verbose == 1) Serial.begin(9600);
  pinMode(mButtonPin, INPUT);
  pinMode(pButtonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (digitalRead(mButtonPin)) hb -= hbChangePerLoop;
  if (digitalRead(pButtonPin)) hb += hbChangePerLoop;
  hb = max(minPulse, min(hb, maxPulse));
  if (verbose == 1) Serial.println(hb);
  
  analogWrite(ledPin, 255 * pow(sin(t)/2+0.5, 4));
  
  t += cycleTime * hb * tauPerMinute;
  delay(cycleTime);
}


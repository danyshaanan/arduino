
#include <math.h>

const int fadeLed = 9;    // this <-> 330 resistor <-> led <-> ground
const int sensorPin = A0; // sensorLegOfPhotoTransistor <-> this <-> 5k resistor <-> power
                          // powerLegOfPhotoTransistor <-> 240 resistor <-> power
                          // groundLegsOfPhotoTransistor <-> ground

const int cycle = 30;
const int verbose = 1;

double lightness = 0;

/////////////////////////////

void setup()  {
  if (verbose) Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(fadeLed, OUTPUT);
}

void loop()  {
  int inUse = analogRead(sensorPin) < 700;
  if (verbose) Serial.println(analogRead(sensorPin));
  lightness += cycle * (inUse ? 0.0007 : -0.0001);
  lightness = max(0, min(lightness, 1));
  analogWrite(fadeLed, lightness * 255);
  digitalWrite(LED_BUILTIN, inUse);
  delay(cycle);
}

// https://www.sparkfun.com/products/9269

#include <math.h>

#define LED            LED_BUILTIN
#define X              0  //analog pin
#define Y              1  //analog pin
#define Z              10 //analog pin
#define VERBOSE        1

int x, y, z;

int minZ = 1024;
int maxZ = 0;

void setup() {
  delay(1000);
  pinMode(LED, OUTPUT);
  if (VERBOSE) Serial.begin(9600);
}

void loop() {
  z = analogRead(Z);
  minZ = min(minZ, z);
  maxZ = max(maxZ, z);
  z = abs(map(z, minZ, maxZ, 0, 255));
  
  analogWrite(LED, z);

  if (VERBOSE) Serial.println(z);
  delay(100);
}


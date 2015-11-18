// https://www.sparkfun.com/products/9269

#include <math.h>

#define X              1 //analog pin
#define Y              2 //analog pin
#define Z              3 //analog pin
#define VERBOSE        1

int x, y, z;

int minVal = 271; //find this value with the calibration lines
int maxVal = 450; //find this value with the calibration lines

//minVal = 1024; //calibration
//maxVal = 0; //calibration

void printXYZ(int x, int y, int z) {
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(z);
  Serial.println("");
}

void setup() {
  if (VERBOSE) Serial.begin(9600);
}

void loop() {
  x = analogRead(X);
  y = analogRead(Y);
  z = analogRead(Z);
//  minVal = min(min(min(minVal, x), y), z); //calibration
//  maxVal = max(max(max(maxVal, x), y), z); //calibration
//  printXYZ(minVal, maxVal, -1); //calibration

  x = map(x, minVal, maxVal, -90, 90);
  y = map(y, minVal, maxVal, -90, 90);
  z = map(z, minVal, maxVal, -90, 90);
  if (VERBOSE) printXYZ(x, y, z);
  delay(100);
}


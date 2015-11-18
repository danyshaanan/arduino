/*
Latching Hall Effect Sensor - Melexis US1881
http://www.melexis.com/Hall-Effect-Sensor-ICs/Hall-Effect-Latches/Hall-Latch---High-Sensitivity-140.aspx

Built and tested on A-Star 32U4 Micro (Leonardo), with Arduino 1.6.3
https://www.pololu.com/product/3101

Connect the US1881 legs to Vcc, ground and A1.
*/

#define led            13 // build in led
#define analogInPin    A1
#define VERBOSE        1

boolean magnetState;

boolean getMagnetState(int pin) {
  return analogRead(pin) < 512;
}

////////////////////////////////////////////////////////

void setup() {
  if (VERBOSE) Serial.begin(9600);
  pinMode(analogInPin, INPUT_PULLUP);
}


void loop() {
  magnetState = getMagnetState(analogInPin);

  if (VERBOSE) Serial.println(magnetState);
  analogWrite(led, magnetState ? 255 : 10);

  delay(100);
}


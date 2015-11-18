// shift register example
// 74HC595

#include <math.h>

#define LEDS 7

byte i;
int shiftRegister[] = { 4, 5, 6 }; // data, latch, clock

void setShiftRegister(int bitCode, int ms) {
   digitalWrite(shiftRegister[1], LOW);
   shiftOut(shiftRegister[0], shiftRegister[2], LSBFIRST, bitCode);
   digitalWrite(shiftRegister[1], HIGH);
   delay(ms);
}

//////////////////////////////////////

void setup() {
  for (i = 0; i < 3; i++) {
    pinMode(shiftRegister[i], OUTPUT);
  }
}

void loop() {
  for (i = pow(2, LEDS); i > 0 ; i--) {
    setShiftRegister(i, 1000);
  }
}

//////////////////////////////////////

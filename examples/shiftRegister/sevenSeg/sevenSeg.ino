// shift register with 7-seg display example
// 74HC595
// https://www.sparkfun.com/products/9191

#include <math.h>

#define LEDS 7

//byte digitsBitCodes[] = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000010,0b11111000,0b10000000,0b10011000,0b01111111};
byte digitsBitCodes[] = { 64, 121, 36, 48, 25, 18, 2, 120, 0, 24, 127 };

int i;
int shiftRegister[] = { 4, 5, 6 }; // data, latch, clock

void setDigit(byte digit, int ms) {
   digitalWrite(shiftRegister[1], LOW);
   shiftOut(shiftRegister[0], shiftRegister[2], LSBFIRST, digitsBitCodes[digit]);
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
  for (i = 10; i >= 0 ; i--) {
    setDigit(i, 1000);
  }
}

//////////////////////////////////////

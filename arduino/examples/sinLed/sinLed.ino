/*

Sin LED Fade

Built and tested on A-Star 32U4 Micro (Leonardo), with Arduino 1.6.3
https://www.pololu.com/product/3101

No wiring required.
For using an external LED, connect it to ground and to a PWN pin through a resistor.

*/

#include <math.h>       /* sin */

#define led            13 // must be a pwm pin.
#define VERBOSE        1

const double tau = 2 * 3.14159265359;
const double periodsPedSecond = 0.333;

double x, seq;

/////////////////////////////

void setup()  {
  pinMode(led, OUTPUT);
}

void loop()  {
  x = float(millis()) / 1000 * tau * periodsPedSecond;
  seq = sin(x) / 2 + 0.5;
  analogWrite(led, seq * 255);
  delay(20);
}

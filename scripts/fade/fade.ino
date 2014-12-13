
#include <math.h>       /* sin */

const int led = 9; // must be a pwm pin.
const double tau = 2 * 3.14159265359;

/////////////////////////////

void setup()  { 
  pinMode(led, OUTPUT);
} 

void loop()  { 
  double seq = sin(millis() * tau / 1000 / 3) / 2 + 0.5;
  analogWrite(led, seq * 255);
  delay(20);
}


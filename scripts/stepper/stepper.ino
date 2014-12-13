
#include <math.h>       /* sin */
#include <Stepper.h>

#define STEPS 200
Stepper stepper = Stepper(STEPS, 4, 5, 6, 7);
double t = 0;
long step = 0;
long newStep = 0;
int loopLength = 1000;
long second = 1000;
long minute = second * 60;
long hour = minute * 60;
long day = hour * 12;

int verbose = 0;

///////////////////////////////////////////////////////////////////////

void setup() {
  if (verbose == 1) Serial.begin(9600);
  stepper.setSpeed(100);
}

void loop() {
  newStep = - t * STEPS / day;
  if (verbose == 1) Serial.println(newStep - step);
  if (newStep != step) stepper.step(newStep - step);
  step = newStep;
  t += loopLength;
  delay(loopLength);
}






#include <Servo.h>

const int serveo = 10;

Servo myServo;

int angle = 0;


void setup() {
  myServo.attach(serveo);
}

void loop() {
  myServo.write(0);
  delay(1000);
  myServo.write(90);
  delay(1000);
  myServo.write(180);
  delay(1000);
}



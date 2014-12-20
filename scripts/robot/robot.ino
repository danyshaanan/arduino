
// pins setup:
// starting from firstPin:
// 4 pins for front motors, 4 for back motors. In each group:
// 2 for right motor, 2 for left motor. In each group:
// 1 for forward, 1 for backward.
// Meaning firstPin is front right forward, firstPin+7 is back left backward.

const int FRONT = 0;
const int BACK = 1;
const int RIGHT = 0;
const int LEFT = 1;
const int FORWARD = 1;
const int STOP = 0;
const int BACKWARD = -1;
const int firstPin = 2;

void drive(int motor, int dir) {
  digitalWrite(motor * 2 + firstPin, dir == 1);
  digitalWrite(motor * 2 + firstPin + 1, dir == -1);
}

void drive(int frontback, int leftright, int dir) {
  drive(frontback * 2 + leftright, dir);
}

void setup() {
  for (int i = firstPin; i < firstPin+8; i++) {
    pinMode(i, OUTPUT);
  }

  //testing:
  drive(FRONT, RIGHT, FORWARD);
  delay(1000);
  drive(FRONT, RIGHT, STOP);
  drive(FRONT, LEFT, FORWARD);
  delay(1000);
  drive(FRONT, LEFT, STOP);
  drive(BACK, LEFT, FORWARD);
  delay(1000);
  drive(BACK, LEFT, STOP);
  drive(BACK, RIGHT, FORWARD);
  delay(1000);
  drive(BACK, RIGHT, STOP);
  delay(1000);
  drive(BACK, LEFT, FORWARD);
  drive(BACK, RIGHT, FORWARD);
  drive(FRONT, LEFT, FORWARD);
  drive(FRONT, RIGHT, FORWARD);
  delay(1000);
  drive(BACK, LEFT, STOP);
  drive(BACK, RIGHT, STOP);
  drive(FRONT, LEFT, STOP);
  drive(FRONT, RIGHT, STOP);
  delay(1000);
  drive(BACK, LEFT, BACKWARD);
  drive(BACK, RIGHT, BACKWARD);
  drive(FRONT, LEFT, BACKWARD);
  drive(FRONT, RIGHT, BACKWARD);
  delay(1000);
  drive(BACK, LEFT, STOP);
  drive(BACK, RIGHT, STOP);
  drive(FRONT, LEFT, STOP);
  drive(FRONT, RIGHT, STOP);

}


void loop() {
}



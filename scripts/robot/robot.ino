

// A robot with 4 motors for 4 wheels, and a front distance sensor.

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
const int distanceSensor = A0;

const int verbose = 0;

////////////////////////////////////////

void drive(int motor, int dir) {
  digitalWrite(motor * 2 + firstPin, dir == 1);
  digitalWrite(motor * 2 + firstPin + 1, dir == -1);
}

void drive(int frontback, int leftright, int dir) {
  drive(frontback * 2 + leftright, dir);
}

///////////////////////////////////

void all(int dir) {
  for (int i=0; i<4; i++) drive(i, dir);
}

void all(int dir, int ms) {
  all(dir);
  delay(ms);
  all(STOP);
}

void turn(int side) {
  for (int i=0; i<4; i++) drive(i, i%2 == side ? -1 : 1);
}

void turn(int side, int ms) {
  turn(side);
  delay(ms);
  all(STOP);
}

///////////////////////////////////


void setup() {
  if (verbose) Serial.begin(9600);
  pinMode(distanceSensor, INPUT);
  for (int i = firstPin; i < firstPin+8; i++) {
    pinMode(i, OUTPUT);
  }
//  all(FORWARD);
}


int movingForward = 1;

void loop() {
  int sensor = analogRead(distanceSensor);
  if (verbose) Serial.println(sensor);
  int somethingNear = sensor > 300;
  if (movingForward && somethingNear) {
    movingForward = 0;
    all(STOP, 500);
    turn(LEFT);
  } else if (!movingForward && !somethingNear) {
    movingForward = 1;
    all(STOP, 500);
    all(FORWARD);
  }
}





// A robot with 4 motors for 4 wheels, and a front distance sensor.

// pins setup:
// starting from firstPin:
// 4 pins for front motors, 4 for back motors. In each group:
// 2 for right motor, 2 for left motor. In each group:
// 1 for forward, 1 for backward.
// Meaning firstPin is front right forward, firstPin+7 is back left backward.

const int firstPin = 2;
const int distanceSensor = A0;

const int RIGHT    = 0b01100110;
const int FORWARD  = 0b01010101;
const int STOP     = 0b00000000;
const int BACKWARD = 0b10101010;
const int LEFT     = 0b10011001;
//                     |    |||
//                     |    ||`--- firstPin   state
//                     |    |`---- firstPin+1 state
//                     |    `----- firstPin+2 state
//                     |           ...
//                     `---------- firstPin+7 state
const int verbose = 0;

////////////////////////////////////////

int currentCommand = FORWARD;

void drive(int command) {
  currentCommand = command;
  long power = 1;
  for (int i=0; i<8; i++) {
    digitalWrite(i + firstPin, power & command);
    power *= 2;
  }
}

void drive(int command, int ms) {
  drive(command);
  delay(ms);
  drive(STOP);
}

///////////////////////////////////

void setup() {
  if (verbose) Serial.begin(9600);
  pinMode(distanceSensor, INPUT);
  for (int i=0; i<8; i++) pinMode(firstPin + i, OUTPUT);
//  drive(FORWARD);
}


void loop() {
  int sensor = analogRead(distanceSensor);
  if (verbose) Serial.println(sensor);
  int somethingNear = sensor > 300;
  if (currentCommand == FORWARD && somethingNear) {
    drive(STOP, 500);
    drive(LEFT);
  } else if (currentCommand != FORWARD && !somethingNear) {
    drive(STOP, 500);
    drive(FORWARD);
  }
  delay(100);
}



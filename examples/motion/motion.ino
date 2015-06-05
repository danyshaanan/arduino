

const int motionDetectorPin = 9;  // sensor black wire <-> this <-> 10k resistor <-> power

const int verbose = 0;

//////////////////////////

void onMotion() {
  if (verbose) Serial.println("Motion Detected");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(4000);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
  if (verbose) Serial.begin(9600);
  pinMode(motionDetectorPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
  if (digitalRead(motionDetectorPin) == LOW) onMotion();
  delay(100);
}

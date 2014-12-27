

const int analogInPin = A3;
const int verbose = 0;

void setup() {
  if (verbose) Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(analogInPin);
  if (verbose) Serial.println(sensorValue);
  digitalWrite(LED_BUILTIN, sensorValue < 122);
  delay(100);
}

// Touch sensor example on a Teensy

#define VERBOSE 0
#define TOUCHPIN A9 // 23 on teensy

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  if (VERBOSE) Serial.begin(9600);
}

void loop() {
  int t = touchRead(A9);
  if (VERBOSE) Serial.println(t);
  digitalWrite(LED_BUILTIN, t < 512);

  delay(10);
}

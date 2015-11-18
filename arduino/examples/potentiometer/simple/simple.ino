
#define POTPIN         0           // potentiometer analog input pin
#define LEDPIN         LED_BUILTIN
#define VERBOSE        0

int t = 0;

void setup() {
  pinMode(LEDPIN, OUTPUT);
  if (VERBOSE) Serial.begin(9600);
}

void loop() {
  t = analogRead(POTPIN);
  if (VERBOSE) Serial.println(t);
  analogWrite(LEDPIN, map(t, 0, 1023, 0, 255));
  delay(33);
}

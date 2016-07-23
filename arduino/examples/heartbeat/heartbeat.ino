
#define SENSORPIN         A0
#define LEDPIN         LED_BUILTIN

#define numOfBeats 10

int t;
int beats[numOfBeats];
int indexOfBeat = 0;
bool state = false;

int calcBPM() {
  int sum = 0;
  for (int i = 0; i < numOfBeats; i++) {
    sum += beats[i];
  }
  return 60000 * numOfBeats / sum;
}

void setup() {
  delay(3000);
  pinMode(SENSORPIN, INPUT);
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int a = analogRead(SENSORPIN);
  
  if (state && a < 400) {
    beats[indexOfBeat++ % numOfBeats] = millis() - t;
    t = millis();
    state = false;
  } else if (!state && a > 600) {
    state = true;
  }

  digitalWrite(LEDPIN, state);
  
  Serial.println(String(calcBPM()) + " - " + String(a));
  delay(30);
}

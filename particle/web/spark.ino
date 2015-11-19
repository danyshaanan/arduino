
int potPin = A1;
int sensorPin = D2;

void setPutoutPin(int pin, int state) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, state);
}

void setup() {
  setPutoutPin(D0, LOW);
  setPutoutPin(A0, LOW);
  setPutoutPin(A2, HIGH);

  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(potPin, INPUT);
  Spark.publish("boot", "online!");
}


int on = 0;
int potValue = 0;
int oldPotValue = 0;


int ms = 10;
int msSinceLastEvent = 0;


char str[5];

void loop() {

  int now = digitalRead(sensorPin);
  if (on != now) {
      Spark.publish("tick", now ? "buttonUp" : "buttonDown");
      on = now;
  }

  msSinceLastEvent += ms;
  potValue = analogRead(potPin);
  if (potValue < oldPotValue - 50 || oldPotValue + 50 < potValue || msSinceLastEvent > 1000) {
      msSinceLastEvent = 0;
      sprintf(str, "%d", (potValue * 4) % 4000);
      Spark.publish("pot", str);
      oldPotValue = potValue;
  }

  delay(ms);
}

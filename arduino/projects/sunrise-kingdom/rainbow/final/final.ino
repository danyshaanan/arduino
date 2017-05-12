
/*****************************************************************************
  Multiple LPD6803-based RGB LED strips

  ------------,
  powerSupply | vcc ---------- Di[]
  ------------| ground -,
                        |----- Li[]
  --------| ground -----`
  arduino | data[] ----------- Ci[]
  --------| clock[] ---------- St[]

  Due to performance issues of this kinds of strips and the amount of LEDs,
  harsh optimisation of this code dictated that data pins are on 0-4,
  (in order to be all on PORTD), and that all clock pins are on 8.

 *****************************************************************************/

class LPD6803m {
  public:
    uint16_t *pixels;
    uint16_t pixelsInStrip;
    uint8_t dataPin;
    uint8_t clockPin;

    LPD6803m(uint16_t n, uint8_t dp, uint8_t cp);
    void setPixelRGB(uint16_t pixelIndex, uint8_t r, uint8_t g, uint8_t b);
    void setPixelHSV(uint16_t pixelIndex, float h, float s, float v);
    void setPixel(uint16_t pixelIndex, uint16_t c);
    static void displayStrips(LPD6803m *strips, uint8_t num);
    static uint16_t rgbToInt(uint8_t r, uint8_t g, uint8_t b);
    static uint16_t hsvToInt(float h, float s, float v);
    static void tick(uint8_t cp);
};


LPD6803m::LPD6803m(uint16_t n, uint8_t dp, uint8_t cp) {
  pixelsInStrip = n;
  dataPin = dp;
  clockPin = cp;
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pixels = (uint16_t *)malloc(pixelsInStrip * sizeof(uint16_t));
}

void LPD6803m::setPixelRGB(uint16_t pixelIndex, uint8_t r, uint8_t g, uint8_t b) {
  setPixel(pixelIndex, rgbToInt(r, g, b));
}

void LPD6803m::setPixelHSV(uint16_t pixelIndex, float h, float s, float v) {
  setPixel(pixelIndex, hsvToInt(h, s, v));
}

void LPD6803m::setPixel(uint16_t pixelIndex, uint16_t c) {
  if (pixelIndex < pixelsInStrip) pixels[pixelIndex] = c;
}

void LPD6803m::tick(uint8_t cp) {
  digitalWrite(cp, HIGH);
  digitalWrite(cp, LOW);
}

void LPD6803m::displayStrips(LPD6803m *strips, uint8_t num) {
  byte port, n;
  short int i;
  PORTD = B00000000;
  for (byte b = 0; b < 32; b++) tick(strips[0].clockPin);
  for (uint16_t pixelIndex = 0; pixelIndex < strips[0].pixelsInStrip; pixelIndex++) {
    PORTD = B00011111;
    tick(strips[0].clockPin);
    for (i = 14; i >= 0; i--) {
      port = B00000000;
      for (n = 0; n < num; n++) if (1 << i & strips[n].pixels[pixelIndex]) port |= 1 << n;
      PORTD = port;
      tick(strips[0].clockPin);
    }
  }
}

uint16_t LPD6803m::rgbToInt(uint8_t r, uint8_t g, uint8_t b) {
  return (((((g & 0x1F) << 5) | (r & 0x1F)) << 5) | (b & 0x1F));
}

uint16_t LPD6803m::hsvToInt(float h, float s, float v) {
  v *= 31;
  if (s == 0) return rgbToInt(v, v, v);
  h *= 6;
  int i = floor(h);
  float f = h - i;
  float p = 1 - s;
  float q = 1 - s * f;
  float t = 1 - s * (1 - f);
  if (i == 0) return rgbToInt(v * 1, v * t, v * p);
  if (i == 1) return rgbToInt(v * q, v * 1, v * p);
  if (i == 2) return rgbToInt(v * p, v * 1, v * t);
  if (i == 3) return rgbToInt(v * p, v * q, v * 1);
  if (i == 4) return rgbToInt(v * t, v * p, v * 1);
  return             rgbToInt(v * 1, v * p, v * q);
}

/*****************************************************************************

  Actual script

 *****************************************************************************/

#define VERBOSE 1
#define NUM_LEDS 100
#define msPerFrame 30
#define potpin0 A0
#define potpin1 A1
#define potpin2 A10
#define numOfStrips 5
#define presetLength 60
#define numOfGreys 256
#define greysCutOf 100

LPD6803m *strips;

uint16_t halfRainbowPreset[presetLength];
uint16_t fullRainbowPreset[presetLength];
uint16_t halfSunWavePreset[presetLength];
uint16_t greyRainbowPreset[presetLength];

long long temp = 0;
long long frames = 0;
long long totalCalcTime = 0;
int i, n, t, calcTime, toDelay, tpot, dt, style, program, ledMul, strpMul, p;
bool b;

void setup() {
  delay(3000);
  if (VERBOSE) Serial.begin(9600);

  pinMode(potpin0, INPUT);
  pinMode(potpin1, INPUT);
  pinMode(potpin2, INPUT);

  strips = (LPD6803m *)malloc(numOfStrips * sizeof(LPD6803m));
  for (byte n = 0; n < numOfStrips; n++) strips[n] = LPD6803m(NUM_LEDS, n, 8);

  for (i = 0; i < presetLength; i++) {
    fullRainbowPreset[i] = LPD6803m::hsvToInt(1. * i / presetLength, 1, 1);
    halfRainbowPreset[i] = LPD6803m::hsvToInt(1. * i / presetLength, 1, pow(0.6 + 0.4 * sin(6.28 * i / presetLength + 2.0), 5));
    halfSunWavePreset[i] = LPD6803m::hsvToInt(0.08, 1, pow(0.50 + 0.50 * sin(6.28 * i / presetLength), 5));
    greyRainbowPreset[i] = LPD6803m::hsvToInt(0.00, 0, pow(0.55 + 0.45 * sin(6.28 * i / presetLength), 1));
  }
}

// Utils:

int fold(int led) {
  return led < NUM_LEDS / 2 ? NUM_LEDS - led : led;
}

int foldIf(int led, bool b) {
  return b ? fold(led) : led;
}

int mod(long int a, int b) {
  int r = a % b;
  return r < 0 ? r + b : r;
}

// Programs:

void executeProgram(int program) {
  if      (program == 0) {} // random, should not get here.
  
  else if (program == 1) { // Shine
    int index = mod(0.0003 * temp, presetLength);
    int s = map(style, 0, 1024, 1, 5);
    uint16_t c;
    if (s == 1) c = fullRainbowPreset[index];
    if (s == 2) c = halfRainbowPreset[index];
    if (s == 3) c = halfSunWavePreset[index];
    if (s == 4) c = greyRainbowPreset[index];
    for (n = 0; n < numOfStrips; n++) {
      for (i = 0; i < NUM_LEDS; i++) {
        strips[n].setPixel(i, c);
      }
    }
    
  } else if (program == 2) { // snake
    int width = map(style, 0, 1024, 0, 10);
    int index = mod(0.0006 * temp, 500);
    for (n = 0; n < numOfStrips; n++) {
      for (i = 0; i < NUM_LEDS; i++) {
        strips[n].setPixel(n % 2 ? i : NUM_LEDS - i, (floor(index / 100) == n && abs(index % 100 - i) <= width) ? 32767 : 0);
      }
    }
  } else if (program == 3) { // pong
    int width = map(style, 0, 1024, 0, 10);
    int index = mod(-0.00003 * temp, 10);
    for (i = 0; i < NUM_LEDS; i++) {
      if (index < 5) strips[index].setPixel(i, fullRainbowPreset[map(index, 0 - width, 14 - width, 0, presetLength)]);
      else strips[9 - index].setPixel(i, 0);
    }
  } else if (program == 4 || program == 5) { // halfSunWavePreset
    b = program == 5;

    for (n = 0; n < numOfStrips; n++) {
      for (i = 0; i < NUM_LEDS; i++) {
        int index = mod(int(- 0.01 * ledMul * foldIf(i, b) - 0.01 * strpMul * n - 0.0002 * temp), presetLength);
        strips[n].setPixel(i, halfSunWavePreset[index]);
      }
    }
  } else if (program == 6 | program == 7) { // fullRainbowPreset
    b = program == 7;

    for (n = 0; n < numOfStrips; n++) {
      for (i = 0; i < NUM_LEDS; i++) {
        int index = mod(int(- 0.01 * ledMul * foldIf(i, b) - 0.01 * strpMul * n - 0.0002 * temp), presetLength);
        strips[n].setPixel(i, fullRainbowPreset[index]);
      }
    }
  } else if (program == 8 || program == 9) { // halfRainbowPreset
    b = program == 9;

    for (n = 0; n < numOfStrips; n++) {
      for (i = 0; i < NUM_LEDS; i++) {
        int index = mod(int(- 0.01 * ledMul * foldIf(i, b) - 0.01 * strpMul * n - 0.0002 * temp), presetLength);
        strips[n].setPixel(i, halfRainbowPreset[index]);
      }
    }
  } else if (program == 10) for (n = 0; n < numOfStrips; n++) for (i = 0; i < NUM_LEDS; i++) strips[n].setPixel(i, i == n ? 7399 : 0);
}

bool isRandom = false;
long long randomFor = 0;
int randIndex = -1;
int randProgram = 5;

void loop() {

  p = analogRead(potpin1);

  program = floor(p / 100); // program is 0 to 10

  if (program) {
    dt = analogRead(potpin0);
    style = analogRead(potpin2);
  } else {
    int currRandIndex = int(millis() / 1000 / 250); // 250 seconds
    if (randIndex != currRandIndex) {
      randProgram  = (rand() % 9) + 1;
      dt = rand() % 1023;
      dt = dt < 512 ? map(dt, 0, 512, 100, 300) : map(dt, 512, 1024, 700, 900);
      style = rand() % 1023;
      randIndex = currRandIndex;
    }
    program = randProgram;
  }

  ledMul = map(style, 0, 1024, 0, 75);
  strpMul = map(style, 0, 1024, 500, 0);

  t = millis();

  executeProgram(program);

  LPD6803m::displayStrips(strips, numOfStrips);

  calcTime = millis() - t;
  totalCalcTime += calcTime;
  frames++;
  if (VERBOSE) Serial.println(1000. * frames / totalCalcTime);
  toDelay = max(0, msPerFrame - calcTime);
  temp += map(dt, 0, 1024, -100, 100) * (calcTime + toDelay);
  delay(toDelay);

}



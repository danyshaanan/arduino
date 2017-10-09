
// https://www.dipmicro.com/store/GY271-MOD

#include <Wire.h>
#include <FastLED.h>

#define BYTE            255
#define PIN             11
#define NUMPIXELS       12
#define HMC5883L_ADDR 0x1E //0011110b, I2C 7bit address of HMC5883

CRGB leds[NUMPIXELS];


int readDir() {
  return map((Wire.read() << 8) | Wire.read(), -400, 200, -100, 100);
}


int getAzimuth() {
  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x03);
  Wire.endTransmission();

  Wire.requestFrom(HMC5883L_ADDR, 6);

  int x = readDir();
  int z = readDir();
  int y = readDir();

  return (PI + atan2(x, y)) * 360 / (2 * PI);
}


void setup() {
  delay(1000);
  
  Wire.begin();
  Serial.begin(9600);
  
  FastLED.addLeds<WS2812, PIN>(leds, NUMPIXELS);  
}

void loop() {
  int led = NUMPIXELS * getAzimuth() / 360;
  Serial.println(led);
  
  for (int i = 0; i < NUMPIXELS; i++) {
    leds[i] = CHSV(BYTE, 0, led == i ? BYTE/4 : 0);
  }

  FastLED.show();

  delay(250);
}


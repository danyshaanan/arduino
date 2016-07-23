
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Sketch > Include Library > Manage Libraries > filter for 'LiquidCrystal_I2C'

#define ROWS 4
#define COLS 20

LiquidCrystal_I2C lcd(0x27, COLS, ROWS); // "0x27" depends on your component. use http://playground.arduino.cc/Main/I2cScanner


void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  lcd.clear();
  lcd.print("Welcome!");
  lcd.setCursor(0, 1); // second row
  lcd.print(millis() / 1000);
  lcd.setCursor(0, 2); // third row
  lcd.print(rand());
  lcd.setCursor(COLS - 1, ROWS - 1); // last char
  lcd.print(millis() / 1000 % 2 ? 'X' : 'Y');

  delay(1000);
}

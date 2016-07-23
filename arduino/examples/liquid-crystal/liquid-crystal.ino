
// Libraries

#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Sketch > Include Library > Manage Libraries > filter for 'LiquidCrystal_I2C'

// Definitions

#define ROWS 4
#define COLS 20

// Variables

LiquidCrystal_I2C lcd(0x27, COLS, ROWS); // "0x27" depends on your component. use http://playground.arduino.cc/Main/I2cScanner
String site = "http://oct82.com ";
String sentance = "                        Time flies like an arrow; fruit flies like a banana.";
int a, sentanceStart, s = 0;

// Utilities

String leftPad(String str, String padding, int l) {
  return str.length() < l ? leftPad(padding + str, padding, l) : str;
}

String timePad(int n) {
  return leftPad(String(n), "0", 2);
}

void printAt(int row, int col, String str) {
  lcd.setCursor(col, row);
  lcd.print(str);
}

// Main

void setup() {
  delay(3000);
  lcd.init();
  lcd.backlight();
}

void loop() {
  a = analogRead(A0);
  sentanceStart = 7 * s % sentance.length();

  lcd.clear();

  printAt(0, 0, "Welcome!");
  printAt(0, COLS - 5, timePad(s / 60 % 60) + ":" + timePad(s % 60));
  printAt(1, s % COLS, "*");
  printAt(2, 0, sentance.substring(sentanceStart, sentanceStart + COLS));
  printAt(3, 0, leftPad("", ">", COLS * a / 1024));
  printAt(3, 0, String(a));
  printAt(ROWS - 1, COLS - 1, String(site[s % site.length()]));

  delay(1000);
  s++;
}


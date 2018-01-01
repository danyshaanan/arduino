/////////////////////////////////////////////////////////

// 8x32 WS2812 LEDs Langton's Ant simulator
// FastLED version 3.001.006

/////////////////////////////////////////////////////////

#include <FastLED.h>

/////////////////////////////////////////////////////////

#define PIN             11
#define ROWS             8
#define COLS            32
#define L               16 // out of 255
#define MSPERTICK      250

/////////////////////////////////////////////////////////

struct color { byte r, g, b; };

/////////////////////////////////////////////////////////

color black, off, on;
CRGB leds[ROWS * COLS];
bool grid[ROWS * COLS];
int x, y, ticks = 0, dir = 1;

/////////////////////////////////////////////////////////

color linear(color a, color b, float p) {
  color res;
  float q = 1 - p;
  res.r = q * a.r + p * b.r;
  res.g = q * a.g + p * b.g;
  res.b = q * a.b + p * b.b;
  return res;
}

void (*resetFunc)(void) = 0;

int mod(int a, int m) {
  return (a % m + m) % m;
}

int xyToIndex(int row, int col) {
  row = mod(row, ROWS);
  col = mod(col, COLS);
  return ROWS * col + (col % 2 ? ROWS - 1 - row : row);
}

/////////////////////////////////////////////////////////

bool isLedOn(int x, int y) {
  return grid[xyToIndex(x, y)];
}

void fadeLed(int x, int y, color from, color to, int ms = MSPERTICK) {
  int frames = max(ms / 30, 1);
  for (int f = 1; f <= frames; f++) {
    color c = linear(from, to, 1. * f / frames);
    leds[xyToIndex(x, y)] = CRGB(c.g, c.r, c.b);
    FastLED.show();
    delay(ms / frames);
  }
}

void activateLed(int x, int y, int ms = MSPERTICK) {
  fadeLed(x, y, black, off, ms);
}

void toggleLed(int x, int y) {
  isLedOn(x, y) ? fadeLed(x, y, on, off) : fadeLed(x, y, off, on);
  grid[xyToIndex(x, y)] ^= true;
}

void deactivateLed(int x, int y, int ms = MSPERTICK) {
  isLedOn(x, y) ? fadeLed(x, y, on, black, ms) : fadeLed(x, y, off, black, ms);
}

/////////////////////////////////////////////////////////

void setup() {
  switch (analogRead(0) % 6) {
    case 0: off.r = on.g = on.b = L; break;
    case 1: on.r = off.g = on.b = L; break;
    case 2: on.r = on.g = off.b = L; break;
    case 3: on.r = off.g = off.b = L; break;
    case 4: off.r = on.g = off.b = L; break;
    case 5: off.r = off.g = on.b = L; break;
  }

  FastLED.addLeds<WS2812, PIN>(leds, ROWS * COLS);

  for (x = 0; x < ROWS; x++) for (y = 0; y < COLS; y++) activateLed(x, y, 5);
  x = ROWS / 2 - 1;
  y = COLS / 2 - 0;
}

/////////////////////////////////////////////////////////

void loop() {
  toggleLed(x, y);
  
  if (dir % 4 == 0) y--;
  if (dir % 4 == 1) x--;
  if (dir % 4 == 2) y++;
  if (dir % 4 == 3) x++;
  
  dir += isLedOn(x, y) ? 3 : 1;

  ticks++;
  if (ticks == 52) delay (1000);
  if (ticks > 4 * 60 * 60) {
    delay(1000);
    for (x = 0; x < ROWS; x++) for (y = 0; y < COLS; y++) deactivateLed(x, y, 5);
    delay(1000);
    resetFunc();
  }
}

/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////

// 8x32 WS2812 LEDs Langton's Ant simulator
// FastLED version 3.001.006

/////////////////////////////////////////////////////////

#include <FastLED.h>

/////////////////////////////////////////////////////////

#define PIN             11
#define ROWS             8
#define COLS            32
#define LIGHT           16 // out of 255
#define MSPERTICK      333

/////////////////////////////////////////////////////////

struct color { float r, g, b; };
color black, off, on;
color linear(color from, color to, float p) {
  color res;
  float q = 1 - p;
  res.r = q * from.r + p * to.r;
  res.g = q * from.g + p * to.g;
  res.b = q * from.b + p * to.b;
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

CRGB leds[ROWS * COLS];
int grid[ROWS * COLS], x, y, visits, ticks = 0, dir = 2;

/////////////////////////////////////////////////////////

void setLed(int x, int y, color c) {
  leds[xyToIndex(x, y)] = CRGB(LIGHT * c.g, LIGHT * c.r, LIGHT * c.b);
}

void fadeLed(int x, int y, color from, color to, int ms = MSPERTICK) {
  int frames = max(ms / 30, 1);
  for (int f = 1; f <= frames; f++) {
    setLed(x, y, linear(from, to, 1. * f / frames));
    FastLED.show();
    delay(ms / frames);
  }
}

void blinkLed(int x, int y) {
  fadeLed(x, y, black, on, 500);
  fadeLed(x, y, on, black, 500);
}

/////////////////////////////////////////////////////////

void setup() {
  switch (analogRead(0) % 4) {
    case 0: on.r = on.g = on.b = 1; break;
    case 1: off.r = on.g = on.b = 1; break;
    case 2: on.r = off.g = on.b = 1; break;
    case 3: on.r = on.g = off.b = 1; break;
  }

  FastLED.addLeds<WS2812, PIN>(leds, ROWS * COLS);

  blinkLed(-0, -0);
  blinkLed(-0, -1);
  blinkLed(-1, -1);
  blinkLed(-1, -0);

//  for (x = 0; x < ROWS; x++) for (y = 0; y < COLS; y++) grid[x][y] = 0;
  x = ROWS / 2 - 1;
  y = COLS / 2 - 0;
}

/////////////////////////////////////////////////////////

void loop() {
  if (dir % 4 == 0) y--;
  if (dir % 4 == 1) x--;
  if (dir % 4 == 2) y++;
  if (dir % 4 == 3) x++;

  visits = grid[xyToIndex(x, y)]++;
  dir += visits % 2 ? 1 : 3;
  
  if      (!visits)    fadeLed(x, y, black, on);
  else if (visits % 2) fadeLed(x, y, on, off);
  else                 fadeLed(x, y, off, on);

  ticks++;
  if (ticks == 52) delay (3000);
  if (ticks > 3 * 60 * 60) {
    delay(8000);
    resetFunc();
  }
}

/////////////////////////////////////////////////////////

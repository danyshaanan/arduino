#include <FastLED.h>

#define PIN             11
#define ROWS             8
#define COLS            32
#define LIGHT           16 // out of 255
#define ticksPerSecond   30
#define framesPerFade    10

CRGB leds[ROWS * COLS];
int grid[ROWS][COLS], palette, x, y, dir = 0;

int mod(int a, int m) {
  return (a % m + m) % m;
}

void setLed(int row, int col, int r, int g, int b) {
  leds[ROWS * col + (col % 2 ? ROWS - 1 - row : row)] = CRGB(g, r, b);
}

void setup() {
  palette = analogRead(A0) % 12;
  FastLED.addLeds<WS2812, PIN>(leds, ROWS * COLS);
  for (x = 0; x < ROWS; x++) {
    for (y = 0; y < COLS; y++) {
      grid[x][y] = 0;
      setLed(x, y, 0, 0, 0);
    }
  }
  x = ROWS / 2;
  y = COLS / 2;
}

void loop() {
  if (dir % 2) x = mod(x + dir - 2, ROWS);
  else         y = mod(y + dir - 1, COLS);

  int visits = ++grid[x][y];

  dir = mod(dir + (visits % 2 ? 1 : -1), 4);

  for (float t = 0; t <= LIGHT; t += 1.0 * LIGHT / framesPerFade) {
    int updown = visits % 2 ? t : LIGHT - t;
    int upstay = visits < 2 ? t : LIGHT;
    int offoff = 0;

         if (palette ==  0) setLed(x, y, updown, upstay, offoff);
    else if (palette ==  1) setLed(x, y, offoff, updown, upstay);
    else if (palette ==  2) setLed(x, y, upstay, offoff, updown);

    else if (palette ==  3) setLed(x, y, updown, offoff, upstay);
    else if (palette ==  4) setLed(x, y, upstay, updown, offoff);
    else if (palette ==  5) setLed(x, y, updown, offoff, upstay);

    else if (palette ==  6) setLed(x, y, updown, upstay, upstay);
    else if (palette ==  7) setLed(x, y, upstay, updown, upstay);
    else if (palette ==  8) setLed(x, y, upstay, upstay, updown);

    else if (palette ==  9) setLed(x, y, offoff, updown, updown);
    else if (palette == 10) setLed(x, y, updown, offoff, updown);
    else if (palette == 11) setLed(x, y, updown, updown, offoff);

    FastLED.show();
    delay(1000 / ticksPerSecond / framesPerFade);
  }
}

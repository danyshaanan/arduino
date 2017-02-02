#include <FastLED.h>

#define PIN             11
#define ROWS            8
#define COLS            32

CRGB leds[ROWS * COLS];
bool grid[ROWS][COLS];

int dir = 0;
int x = ROWS / 2;
int y = COLS / 2;

// LEDS:

int getLedIndex(int row, int col) {
  return col % 2 ? (ROWS - 1 - row) + ROWS * col : row + ROWS * col;
}

void setLedBrightness(int i, int j, int v) {
  leds[getLedIndex(i, j)] = CRGB(v, v, v);
}

////////////////

void setOffGrid() {
  for (int i = 0; i < ROWS; i++) for (int j = 0; j < COLS; j++) grid[i][j] = false; // rand() % 2;
}

void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);
  FastLED.addLeds<WS2812, PIN>(leds, ROWS * COLS);
  setOffGrid();
}

void loop() {
  
  if (dir == 0) x += 1;
  if (dir == 1) y += 1;
  if (dir == 2) x -= 1;
  if (dir == 3) y -= 1;

  if (x < 0) x += ROWS;
  if (y < 0) y += COLS;
  x %= ROWS;
  y %= COLS;

  bool cell = grid[x][y];
  dir += cell ? 1 : -1;
  
  if (dir < 0) dir += 4;
  dir %= 4;

  grid[x][y] = !cell;
  //setLedBrightness(x, y, cell ? 4 : 0);

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      setLedBrightness(i, j, grid[i][j] ? 4 : 0);
    }
  }

  FastLED.show();
  delay(100);
}



















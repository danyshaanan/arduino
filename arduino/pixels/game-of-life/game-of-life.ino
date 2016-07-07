
#include <FastLED.h>

#define PIN             11
#define ROWS            8
#define COLS            32

int count;

CRGB leds[ROWS * COLS];
bool grid[ROWS][COLS];
bool temp[ROWS][COLS];

int index(int row, int col) {
  return col % 2 ? (ROWS - 1 - row) + ROWS * col : row + ROWS * col;
}

void setLedBrightness(int i, int j, int v) {
  leds[index(i, j)] = CRGB(v, v, v);
}

int countNeighbors(int i, int j) {
  int neighbors = -temp[i][j];
  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      neighbors += temp[(ROWS + i + x) % ROWS][(COLS + j + y) % COLS];
    }
  }
  return neighbors;
}

void setup() {
  srand(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
  FastLED.addLeds<WS2812, PIN>(leds, ROWS * COLS);
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      grid[i][j] = rand() % 2;
    }
  }
}

void loop() {
  digitalWrite(LED_BUILTIN, grid[0][0]);
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      setLedBrightness(i, j, grid[i][j] ? 4 : 0);
      temp[i][j] = grid[i][j];
    }
  }

  FastLED.show();
  delay(100);

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      count = countNeighbors(i, j);
      grid[i][j] = (temp[i][j] && 2 <= count && count <= 3) || (!temp[i][j] && count == 3);
    }
  }
}


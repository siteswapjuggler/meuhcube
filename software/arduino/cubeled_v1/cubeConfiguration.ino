#define VERSION 1
#define SIZE 7

#if (VERSION == 0)
const int layerPins[SIZE] = { 55, 56, 57, 58, 59, 60, 61 };

const int groundPins[SIZE][SIZE] = {
  { 53, 2, 3, 62, 5, 52, 7 },      // ligneA
  { 8, 9, 10, 11, 12, 13, 14 },    // ligneB
  { 15, 16, 17, 18, 19, 20, 21 },  // ligneC
  { 22, 23, 24, 25, 26, 27, 28 },  // ligneD
  { 29, 30, 31, 32, 33, 34, 35 },  // ligneE
  { 36, 37, 38, 39, 40, 41, 42 },  // ligneF
  { 43, 44, 45, 46, 47, 48, 49 }   // ligneG
};
#else
const int layerPins[SIZE] = { 49, 45, 48, 46, 12, 11, 31 };

const int groundPins[SIZE][SIZE] = {
  { 47, 69, 67, 65, 66, 58, 57 },  // X0
  { 43, 68, 64, 63, 56, 55, 54 },  // X1
  { 41, 29, 61, 60, 59, 62, 44 },  // X2
  { 39, 27, 36, 38, 40, 42, 34 },  // X3
  { 35, 25, 24, 26, 28, 30, 32 },  // X4
  { 37, 22,  2,  4,  6,  8, 13 },  // X5
  { 33, 23,  3,  5,  7,  9, 10 }   // X6
};
#endif

byte interLayer = 20;              // time in micros seconds beetween 2 layers activations
byte currentLayer = 0;             // variable pour le cube

byte actualFrame[SIZE][SIZE];      // stocke les valeurs à afficher
byte bufferFrame[SIZE][SIZE];      // prépare les valeurs à afficher

void cubeBegin() {
  // Set layers
  for (int layer = 0; layer < SIZE; layer++) {
    pinMode(layerPins[layer], OUTPUT);
    digitalWrite(layerPins[layer], LOW);
  }

  // Set columns
  for (int line = 0; line < SIZE; line++) {
    for (int column = 0; column < SIZE; column++) {
      pinMode(groundPins[line][column], OUTPUT);
      digitalWrite (groundPins[line][column], LOW);
    }
  }
}

void initLeds() {
  for (int line = 0; line < SIZE; line++) {
    for (int column = 0; column < SIZE; column++) {
      actualFrame[line][column] = B0000000;
    }
  }
}

void testLeds() {
  for (int line = 0; line < SIZE; line++) {
    for (int column = 0; column < SIZE; column++) {
      actualFrame[line][column] = B11111111;
    }
  }
}

void refreshFrame() {
  for (int line = 0; line < SIZE; line++) {
    for (int column = 0; column < SIZE; column++) {
      actualFrame[line][column] = bufferFrame[line][column];
    }
  }  
}

void drawLeds() {
  digitalWrite(layerPins[currentLayer], LOW);     // cut the layer before going to the next one.
  currentLayer++;
  currentLayer %= 7;

  // Refresh columns for the current Layer
  for (int line = 0; line < SIZE; line++) {
    for (int column = 0; column < SIZE; column++) {
      digitalWrite (groundPins[line][column], bitRead(actualFrame[line][column], currentLayer));
    }
  }

  digitalWrite(layerPins[currentLayer], HIGH);    // display the layer.
}


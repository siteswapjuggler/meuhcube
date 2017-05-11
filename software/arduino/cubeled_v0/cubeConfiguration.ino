#define SIZE 7

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

byte currentLayer = 0;             // variable pour le cube
byte cubeVoxels[SIZE][SIZE];       // stocke les valeurs à afficher

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
      cubeVoxels[line][column] = B0000000;
    }
  }
}

void drawLeds() {
  // Refresh columns for the current Layer
  for (int line = 0; line < SIZE; line++) {
    for (int column = 0; column < SIZE; column++) {
      digitalWrite (groundPins[line][column], bitRead(cubeVoxels[line][column], currentLayer));
    }
  }
  
  digitalWrite(layerPins[currentLayer], HIGH);    // display the layer.
  delayMicroseconds(400);                         // make sure enough power get to the led.
  digitalWrite(layerPins[currentLayer], LOW);     // cut the layer before going to the next one.

  currentLayer++;
  currentLayer %= 7;
}

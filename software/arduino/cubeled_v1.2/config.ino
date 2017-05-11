//------------------------------------------
// configuration des pattes
//------------------------------------------

#if (VERSION == 0)
const int zPins[SIZE] = { 55, 56, 57, 58, 59, 60, 61 };
const int xyPins[SIZE][SIZE] = {
  { 53, 2, 3, 62, 5, 52, 7 },      // ligneA
  { 8, 9, 10, 11, 12, 13, 14 },    // ligneB
  { 15, 16, 17, 18, 19, 20, 21 },  // ligneC
  { 22, 23, 24, 25, 26, 27, 28 },  // ligneD
  { 29, 30, 31, 32, 33, 34, 35 },  // ligneE
  { 36, 37, 38, 39, 40, 41, 42 },  // ligneF
  { 43, 44, 45, 46, 47, 48, 49 }   // ligneG
};
#else
const int zPins[SIZE] = { 49, 45, 48, 46, 12, 11, 31 };
const int xyPins[SIZE][SIZE] = {
  { 47, 69, 67, 65, 66, 58, 57 },  // X0
  { 43, 68, 64, 63, 56, 55, 54 },  // X1
  { 41, 29, 61, 60, 59, 62, 44 },  // X2
  { 39, 27, 36, 38, 40, 42, 34 },  // X3
  { 35, 25, 24, 26, 28, 30, 32 },  // X4
  { 37, 22,  2,  4,  6,  8, 13 },  // X5
  { 33, 23,  3,  5,  7,  9, 10 }   // X6
};
#endif

const byte mask = 0xFF >> (8 - SIZE); // mask pour éviter les bits perdus

//------------------------------------------
// VARIABLES GLOBALES
//------------------------------------------

byte actualFrame[SIZE][SIZE];       // stocke les valeurs à afficher
byte bufferFrame[SIZE][SIZE];       // prépare les valeurs à afficher
byte currentLayer = 0;              // variable pour global qui stocke la couche à afficher

//------------------------------------------
// INTIALISATIONS
//------------------------------------------

void cubeBegin() {
  // Set layers
  for (int layer = 0; layer < SIZE; layer++) {
    pinMode(zPins[layer], OUTPUT);
    digitalWrite(zPins[layer], LOW);
  }

  // Set columns
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      pinMode(xyPins[x][y], OUTPUT);
      digitalWrite (xyPins[x][y], LOW);
    }
  }
}

void initLeds() {
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      bufferFrame[x][y] = 0;
    }
  }
}

//------------------------------------------
// AFFICHAGE
//------------------------------------------

void refreshFrame() {
  //TODO ici mode automatique
  copyFrame(bufferFrame, actualFrame);
  changed = false;
}

void drawLeds() {
  digitalWrite(zPins[currentLayer], LOW);     // cut the layer before going to the next one.
  currentLayer++;
  currentLayer %= SIZE;

  // Refresh columns for the current Layer
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      digitalWrite (xyPins[x][y], bitRead(actualFrame[x][y], currentLayer));
    }
  }

  digitalWrite(zPins[currentLayer], HIGH);    // display the layer.
}

//------------------------------------------
// UTILITAIRES
//------------------------------------------

void testLeds() {
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      bufferFrame[x][y] = mask;
    }
  }
  changed = true;
}

void copyFrame(byte source[][SIZE], byte destination[][SIZE]) {
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      destination[x][y] = source[x][y];
    }
  }
}


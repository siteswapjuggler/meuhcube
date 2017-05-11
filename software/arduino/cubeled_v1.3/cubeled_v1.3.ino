/*
   CUBE 7x7x7 - version 1.3
   by Sylvain Garnavault, Luc Bénéchet, Vincent Lambert, Kévin le Déroff

*/

#define VERSION 1                 // version 0 pour le cube de Luc, version 1 pour le Meuhcube
#define SIZE 7

//------------------------------------------
// VARIABLES GLOBALES
//------------------------------------------

byte actualFrame[SIZE][SIZE];       // stocke les valeurs à afficher
byte bufferFrame[SIZE][SIZE];       // prépare les valeurs à afficher
byte currentLayer = 0;              // variable pour global qui stocke la couche à afficher

//----------------------------------------
// VARIABLES
//----------------------------------------

bool changed = false;             // booléen qui indique si buffer a été modifié depuis dernier affichage
bool modedem = false;             // booléen qui détermine si on est en mode démo
long elapsed = 0;                 // time control for refreshing
long pTime   = 0;                 // time control for demo mode
int  compteur = 0;                // pour le mode demo

bool refresh;                     // booléen qui indique si on utilise un rafraichissement externe ou automatique
byte interFrame;                  // time in millis seconds beetween 2 frame drawing
int  interLayer;                  // time in micros seconds beetween 2 layers activations

//----------------------------------------
// INITIALISATION
//----------------------------------------

void setup() {
  Serial.begin(115200);
  Serial.println("Meuhcube online");

#if (VERSION == 0)
  modedem = demo();
  Serial.println(modedem ? "Mode Demo : ON" : "Mode Demo : OFF");
#endif

  cubeBegin();
  initLeds();
  loadParams();
  
  //testLeds();
}

//----------------------------------------
// BOUCLE PRINCIPALE
//----------------------------------------

void loop() {
  if (refresh && changed) refreshFrame();

  if (interLayer) {
    long t = micros();
    if (t - elapsed >= interLayer) {
      drawLeds();
      elapsed = t;
    }
  }
  else drawLeds();

  if (modedem && (millis() - pTime > 50)) {
    compteur++;
    if (compteur < 100) {
      byte x = random(7);
      byte y = random(7);
      byte z = random(7);
      bitWrite(actualFrame[x][y], z, 1);
    }
    else if (compteur < 200) {
      byte x = random(7);
      byte y = random(7);
      byte z = random(7);
      bitWrite(actualFrame[x][y], z, 0);
    }
    else {
      compteur = 0;
    }
    pTime = millis();
  }
}

//----------------------------------------
// GESTION DES EVENEMENTS SERIE
//----------------------------------------

void serialEvent() {
  if (!modedem) {
    while (Serial.available()) {
      byte input = Serial.read();
      parse(input);
    }
  }
}

/*
   CUBE 7x7x7 - version 1.2
   by Sylvain Garnavault, Vincent Lambert, Luc Bénéchet, Kévin le Déroff

*/

#define VERSION 1                 // version 0 pour le cube de Luc, version 1 pour le Meuhcube
#define SIZE 7

//----------------------------------------
// VARIABLES
//----------------------------------------

bool changed = false;             // booléen qui indique si buffer a été modifié depuis dernier affichage
long elapsed = 0;                 // time control for refreshing

bool refresh;                     // booléen qui indique si on utilise un rafraichissement externe ou automatique
byte interFrame;                  // time in millis seconds beetween 2 frame drawing
int  interLayer;                  // time in micros seconds beetween 2 layers activations

//----------------------------------------
// INITIALISATION
//----------------------------------------

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, je suis le Meuhcube v1.2");
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
}

//----------------------------------------
// GESTION DES EVENEMENTS SERIE
//----------------------------------------

void serialEvent() {
  while (Serial.available()) {
    byte input = Serial.read();
    parse(input);
  }
}

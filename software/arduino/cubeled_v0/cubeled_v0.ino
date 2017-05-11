/*   CUBE 7x7x7 - version 0.1
 *   by Sylvain Garnavault, Vincent Lambert, Luc Bénéchet, Kévin le Déroff
 */

//----------------------------------------
// INITIALISATION
//----------------------------------------

void setup() {
  Serial.begin(115200);
  cubeBegin();
  initLeds();
}

//----------------------------------------
// BOUCLE PRINCIPALE
//----------------------------------------

void loop() {
 drawLeds();
}

//----------------------------------------
// GESTION DES EVENEMENTS SERIE
//----------------------------------------

void serialEvent() {
  while (Serial.available()) {
    byte input = Serial.read();
    parseInput(input);
  }
}

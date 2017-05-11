/*   CUBE 7x7x7 - version 0.2
 *   by Sylvain Garnavault, Vincent Lambert, Luc Bénéchet, Kévin le Déroff
 */

//----------------------------------------
// INITIALISATION
//----------------------------------------

void setup() {
  Serial.begin(115200);
  cubeBegin();
  initLeds();
  //testLeds();
  //testMosfet1();
}

//----------------------------------------
// BOUCLE PRINCIPALE
//----------------------------------------

void loop() {
  if (refresh) refreshFrame();
  if (elapsed) drawLeds();
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

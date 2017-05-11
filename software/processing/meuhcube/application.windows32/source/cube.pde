//----------------------------------------------------------------------------------
// La structure des trames est la suivante : SOP, CMD, LEN, (DATA), EOP
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Fonctions de base 
//----------------------------------------------------------------------------------

// @desc  : change la valeur de toute les couches du cube 
// @params: led_states [0-0x7F] correpondant à l'allumage des couches, bit 0 : couche 1 ... bit 6 : couche 7

void set_all(int val) {
  byte[] set_all = {(byte)0xE7, (byte)0x00, (byte)0x01, (byte)val, (byte)0x7E};
  myPort.write(set_all);
}

// @desc  : change la valeur d'un pixel
// @params: x [0-6], y [0-6], z [0-6], bool [0-1] 

void set_pixel(int x, int y, int z, int val) {
  byte[] set_pixel = {(byte)0xE7, (byte)0x01, (byte)0x04, (byte)x, (byte)y, (byte)z, (byte)val, (byte)0x7E};
  myPort.write(set_pixel);
}

// @desc   : change la valeur d'une ligne sur l'un des axes de références
// @params : axe [0-2], coord1 [0-6], coord2 [0-6], led_states [0-0x7F]
// @details: axe(0) > X (c1 = z, c2 =y), axe(1) > (c1 = x, c2 = y), axe(2) > (c1 = x, c2 = z) 

void set_line(int axe, int c1, int c2, int val) {
  byte[] set_line = {(byte)0xE7, (byte)0x02, (byte)0x04, (byte)axe, (byte)c1, (byte)c2, (byte)val, (byte)0x7E};
  myPort.write(set_line);
}

// @desc   : change la valeur de toute les leds d'un plan
// @params : plan [0-2], coord1 [0-6], led_states [0-0x7F]

void set_plan(int plan, int c1, byte[] val) {
  byte[] entete = {(byte)0xE7, (byte)0x03, (byte)0x04, (byte)plan, (byte)c1};
  myPort.write(entete);
  int len = val.length;
  if (len == 1 || len == 9) {
    myPort.write(val);
  }
  myPort.write((byte)0x7E);
}

//----------------------------------------------------------------------------------
// Fonctions d'inversion
//----------------------------------------------------------------------------------

void rev_all() {
  byte[] rev_all = {(byte)0xE7, (byte)0x10, (byte)0x00, (byte)0x7E};
  myPort.write(rev_all);
}

void rev_pixel(int x, int y, int z, int val) {
  // plan 0 = ligne en z, plan 1 = ligne en x, plan 2 = ligne en y
  byte[] rev_pixel = {(byte)0xE7, (byte)0x11, (byte)0x04, (byte)x, (byte)y, (byte)z, (byte)val, (byte)0x7E};
  myPort.write(rev_pixel);
}

void rev_line(int plan, int c1, int c2, int val) {
  // plan 0 = ligne en z, plan 1 = ligne en x, plan 2 = ligne en y
  byte[] rev_line = {(byte)0xE7, (byte)0x12, (byte)0x04, (byte)plan, (byte)c1, (byte)c2, (byte)val, (byte)0x7E};
  myPort.write(rev_line);
}


//----------------------------------------------------------------------------------
// Fonctions de mouvement
//----------------------------------------------------------------------------------

void set_plan(int plan, int num, int val) {
  byte[] set_plan   = {(byte)0xE7, (byte)0x03, (byte)0x03, (byte)plan, (byte)num, (byte)val, (byte)0x7E};
  myPort.write(set_plan);
}


//----------------------------------------------------------------------------------
// Fonctions de mouvement
//----------------------------------------------------------------------------------

void move(int val) {
  byte[] move   = {(byte)0xE7, (byte)0x21, (byte)0x01, (byte)val, (byte)0x7E};
  myPort.write(move);
}


//----------------------------------------------------------------------------------
// Fonctions de composition 
//----------------------------------------------------------------------------------

void cube(int taille) {
  int val, c1, c2;  
  switch (taille) {
  case 0 : 
    val = 8; 
    c1 = 3; 
    c2 = 3; 
    break;
  case 1 : 
    val = 28; 
    c1 = 2; 
    c2 = 4; 
    break;
  case 2 : 
    val = 62; 
    c1 = 1; 
    c2 = 5; 
    break;
  default : 
    val = 127; 
    c1 = 0; 
    c2 = 6; 
    break;
  }

  for (int i=0; i<3; i++) {
    set_line(i, c1, c1, val);
    set_line(i, c1, c2, val);
    set_line(i, c2, c1, val);
    set_line(i, c2, c2, val);
  }
}

void randomPixel(int val) {
  int x = int(random(7));
  int y = int(random(7));
  int z = int(random(7));
  set_pixel(x, y, z, val);
}

//----------------------------------------------------------------------------------
// Fonctions de composition 
//----------------------------------------------------------------------------------

float volume() {
  float val = 1.5*255.*(in.left.level()+in.right.level())/2;
  val = pow(2, constrain(int(val), 0, 255)>>5)-1;
  return val;
}
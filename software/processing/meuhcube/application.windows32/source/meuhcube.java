import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import controlP5.*; 
import ddf.minim.*; 
import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class meuhcube extends PApplet {

                               // biblioth\u00e8que pour l'interface graphique
ControlP5 cp5;

                               // biblioth\u00e8que Audio
Minim minim;

AudioInput in;


Serial    myPort = null;                          // create object from Serial class
String[]  portNames;
final int BAUD_RATE = 115200;

boolean   connected = false;

public void setup() 
{
  
  
  // filter com port names
  portNames = Serial.list();
  String[] filteredNames = {};
  for (int i=0; i<portNames.length; i++) {
    String[] match = match(portNames[i], "/dev/cu.*|COM*");
    if (match != null) {
      filteredNames = append(filteredNames, portNames[i]);
    }
  }
  portNames = filteredNames;

  // interface
  cp5 = new ControlP5(this);

  cp5.addButton("Reverse")
    .setPosition(200, 20)
    .setSize(80, 20);

  cp5.addButton("Cubes")
    .setPosition(200, 45)
    .setSize(80, 20);

  cp5.addButton("Stop")
    .setPosition(200, 70)
    .setSize(80, 20);

  cp5.addButton("Micro")
    .setPosition(200, 95)
    .setSize(80, 20);

  cp5.addTextfield("OSC Server")
    .setPosition(20, 50)
    .setSize(160, 20)
    .setAutoClear(false);

  cp5.addTextfield("OSC Port")
    .setPosition(20, 90)
    .setSize(160, 20)
    .setAutoClear(false);

  cp5.addScrollableList("Comport")
    .setPosition(20, 20)
    .setSize(160, 300)
    .setBarHeight(20)
    .setItemHeight(20)
    .addItems(portNames)
    .setType(ScrollableList.DROPDOWN)
    .setOpen(false);

  // Audio configuration
  minim = new Minim(this);
  in = minim.getLineIn();

  frameRate(25);
}

boolean micOn = false;

//----------------------------------------------------------------------------------
// Boucle principale
//----------------------------------------------------------------------------------

public void draw() {
  background(0);
  if (connected&&micOn) {
    move(0);
    set_plan(0, 0, (int)volume());
  }
}

//----------------------------------------------------------------------------------
// Actions des boutons
//----------------------------------------------------------------------------------

public void Reverse() {
  rev_all();
}

public void Cubes() {
  cube(1);
  cube(3);
}

public void Stop() {
  set_all(0);
}

public void Micro() {
  if (micOn) micOn = false;
  else micOn = true;
}

public void Comport(int n) {
  connected = false;

  // close actual serial port
  if (myPort != null) {
    myPort.stop();
    myPort = null;
  }

  // try open new serial port or report error
  try {
    myPort = new Serial(this, portNames[n], BAUD_RATE);
  }
  catch(Exception e) {
    System.err.println("Error opening serial port " + portNames[n]);
  }
}

//----------------------------------------------------------------------------------
// Actions des boutons
//----------------------------------------------------------------------------------

String myString = null;   // store the line
int lf = 10;              // Linefeed in ASCII

public void serialEvent(Serial myPort) {
  myString = myPort.readStringUntil(lf);
  if (myString != null)
  {
    print(myString);
    String[] match = match(myString, "Hello, je suis le Meuhcube v1.2*");

    if (match != null) {
      println("Meuhcube connect\u00e9...");
      connected = true;
    }
  }
}
//----------------------------------------------------------------------------------
// La structure des trames est la suivante : SOP, CMD, LEN, (DATA), EOP
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Fonctions de base 
//----------------------------------------------------------------------------------

// @desc  : change la valeur de toute les couches du cube 
// @params: led_states [0-0x7F] correpondant \u00e0 l'allumage des couches, bit 0 : couche 1 ... bit 6 : couche 7

public void set_all(int val) {
  byte[] set_all = {(byte)0xE7, (byte)0x00, (byte)0x01, (byte)val, (byte)0x7E};
  myPort.write(set_all);
}

// @desc  : change la valeur d'un pixel
// @params: x [0-6], y [0-6], z [0-6], bool [0-1] 

public void set_pixel(int x, int y, int z, int val) {
  byte[] set_pixel = {(byte)0xE7, (byte)0x01, (byte)0x04, (byte)x, (byte)y, (byte)z, (byte)val, (byte)0x7E};
  myPort.write(set_pixel);
}

// @desc   : change la valeur d'une ligne sur l'un des axes de r\u00e9f\u00e9rences
// @params : axe [0-2], coord1 [0-6], coord2 [0-6], led_states [0-0x7F]
// @details: axe(0) > X (c1 = z, c2 =y), axe(1) > (c1 = x, c2 = y), axe(2) > (c1 = x, c2 = z) 

public void set_line(int axe, int c1, int c2, int val) {
  byte[] set_line = {(byte)0xE7, (byte)0x02, (byte)0x04, (byte)axe, (byte)c1, (byte)c2, (byte)val, (byte)0x7E};
  myPort.write(set_line);
}

// @desc   : change la valeur de toute les leds d'un plan
// @params : plan [0-2], coord1 [0-6], led_states [0-0x7F]

public void set_plan(int plan, int c1, byte[] val) {
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

public void rev_all() {
  byte[] rev_all = {(byte)0xE7, (byte)0x10, (byte)0x00, (byte)0x7E};
  myPort.write(rev_all);
}

public void rev_pixel(int x, int y, int z, int val) {
  // plan 0 = ligne en z, plan 1 = ligne en x, plan 2 = ligne en y
  byte[] rev_pixel = {(byte)0xE7, (byte)0x11, (byte)0x04, (byte)x, (byte)y, (byte)z, (byte)val, (byte)0x7E};
  myPort.write(rev_pixel);
}

public void rev_line(int plan, int c1, int c2, int val) {
  // plan 0 = ligne en z, plan 1 = ligne en x, plan 2 = ligne en y
  byte[] rev_line = {(byte)0xE7, (byte)0x12, (byte)0x04, (byte)plan, (byte)c1, (byte)c2, (byte)val, (byte)0x7E};
  myPort.write(rev_line);
}


//----------------------------------------------------------------------------------
// Fonctions de mouvement
//----------------------------------------------------------------------------------

public void set_plan(int plan, int num, int val) {
  byte[] set_plan   = {(byte)0xE7, (byte)0x03, (byte)0x03, (byte)plan, (byte)num, (byte)val, (byte)0x7E};
  myPort.write(set_plan);
}


//----------------------------------------------------------------------------------
// Fonctions de mouvement
//----------------------------------------------------------------------------------

public void move(int val) {
  byte[] move   = {(byte)0xE7, (byte)0x21, (byte)0x01, (byte)val, (byte)0x7E};
  myPort.write(move);
}


//----------------------------------------------------------------------------------
// Fonctions de composition 
//----------------------------------------------------------------------------------

public void cube(int taille) {
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

public void randomPixel(int val) {
  int x = PApplet.parseInt(random(7));
  int y = PApplet.parseInt(random(7));
  int z = PApplet.parseInt(random(7));
  set_pixel(x, y, z, val);
}

//----------------------------------------------------------------------------------
// Fonctions de composition 
//----------------------------------------------------------------------------------

public float volume() {
  float val = 1.5f*255.f*(in.left.level()+in.right.level())/2;
  val = pow(2, constrain(PApplet.parseInt(val), 0, 255)>>5)-1;
  return val;
}
/*
byte[][][] cube; 

class explosion {
   int   t;
   int[] pos;
   int[] offset;
   boolean finished;
   
   explosion(int _x, int _y, int _z) {
     pos = {_x,_y,_z};
     offset = {0,0,0};
     t = 0;
     finished = false;
   }
   
   void update() {
     if (!finished) t++;
     int[] coord = {x,y,z};
     int th = max(6-max(coord),6-min(coord));
     if (t > th) finished = true;
   }
   
   void display() {
     if(!finished) {
       if (t == 0) {
         cube[x][y][z] = 1;
       }
       else {
         int[] offset = {t,0,0};
         
         if (visible
       }
       t++;
     }
   }
   
   boolean visible() {
     if (x+offset[0] < 0 && x+offset[0] >= 7) {
       return false;
     }
     if (y+offset[1] < 0 && y+offset[1] >= 7) {
       return false;
     }
     if (z+offset[2] < 0 && z+offset[2] >= 7) {
       return false;
     }
     return true;
   }
   
   boolean finished() {
     return finished;
   }
}
*/
  public void settings() {  size(300, 135); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "--present", "--window-color=#666666", "--stop-color=#cccccc", "meuhcube" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}

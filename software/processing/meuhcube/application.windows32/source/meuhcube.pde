import controlP5.*;                               // bibliothèque pour l'interface graphique
ControlP5 cp5;

import ddf.minim.*;                               // bibliothèque Audio
Minim minim;

AudioInput in;

import processing.serial.*;
Serial    myPort = null;                          // create object from Serial class
String[]  portNames;
final int BAUD_RATE = 115200;

boolean   connected = false;

void setup() 
{
  size(300, 135);
  
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

void draw() {
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

void Comport(int n) {
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

void serialEvent(Serial myPort) {
  myString = myPort.readStringUntil(lf);
  if (myString != null)
  {
    print(myString);
    String[] match = match(myString, "Hello, je suis le Meuhcube v1.2*");

    if (match != null) {
      println("Meuhcube connecté...");
      connected = true;
    }
  }
}
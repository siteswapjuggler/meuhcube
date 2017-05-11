import processing.serial.*;
import controlP5.*;

ControlP5 cp5;

Serial    serialPort = null;
String[]  portNames;
final int BAUD_RATE = 115200;

void setup() {
  size(800, 600);
  portNames = Serial.list();

  String[] filteredNames = {};

  for (int i=0; i<portNames.length; i++) {
    String[] match = match(portNames[i], "/dev/cu.*");
    if (match != null) {
      filteredNames = append(filteredNames, portNames[i]);
    }
  }

  portNames = filteredNames;

  cp5 = new ControlP5(this);
  cp5.addScrollableList("dropdown")
    .setPosition(20, 20)
    .setSize(160, 300)
    .setBarHeight(20)
    .setItemHeight(20)
    .addItems(portNames)
    .setType(ScrollableList.DROPDOWN);
}


void dropdown(int n) {
  if (serialPort != null) {
    serialPort.stop();
    serialPort = null;
  }

  String portName = portNames[n];

  try {
    serialPort = new Serial(this, portName, BAUD_RATE);
  }
  catch(Exception e) {
    System.err.println("Error opening serial port " + portName);
  }
}

void draw() {
  background(128);
}

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
    }
  }
}
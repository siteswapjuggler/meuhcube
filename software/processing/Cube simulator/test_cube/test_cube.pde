int n = 7;
led[][][] cube;

void setup() {
  size(800,600, P3D);
  background(0);
  noStroke();
  smooth();
  
  hint(ENABLE_DEPTH_TEST);
  hint(ENABLE_DEPTH_SORT);

  float e = 30.;

  cube = new led[n][n][n];
  for (float i=0; i<n; i++) {
    for (float j=0; j<n; j++) {
      for (float k=0; k<n; k++) {
        cube[(int)i][(int)j][(int)k] = new led(width*0.5+(i-(n-1)*0.5)*e,height*0.5+(j-(n-1)*0.5)*e,-k*e);
      }
    }
  }
}

void draw() {
  clear();
  lights();
  //println(frameRate);
  camera(600,100, 400.0, width/2, height/2, 0.0, 0.0, 1.0, 0.0);
  
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      for (int k=0; k<n; k++) {
        cube[i][j][k].display();
      }
    }
  }
  
  cube[int(random(7))][int(random(7))][int(random(7))].rev();
  
  /*
  pushMatrix();
  fill(0,128,255,16);
  translate(width*0.5,height*0.5,-(n+2)*30/2 + 30);
  box((n+2)*30,(n+2)*30,-(n+2)*30);
  popMatrix();
  */
}
  
class led {
  float x,y,z;
  boolean s;
  
  led() {
    x = width*0.5;
    y = height*0.5;
    z = 0;
    s = false;
  }
  
  led(float xpos, float ypos, float zpos) {
    x = xpos;
    y = ypos;
    z = zpos;
    s = false;
  }
  
  void set(boolean _s) {
    s = _s;
  }
  
  void rev() {
    s = s ? false : true;
  }
  
  void display() {
    pushMatrix();
    translate(x,y,z);
    if (s) fill(0,128,255,96);
    else   fill(0,128,255,16);
    sphereDetail(4);
    sphere(5);
    popMatrix();
  }  
}
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
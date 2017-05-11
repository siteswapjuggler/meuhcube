nbled = 7;
ecart = 16.5;
fil   = 0.5;
depth = 3;

linear_extrude(height = depth)
difference() {  
    square([nbled*ecart,ecart+fil]);

for (i = [1:nbled]) {
    translate([i*ecart - ecart/2 - fil,0,0]) 
    square([fil*2,fil*2]);
  }

for (i = [1:nbled]) {
    translate([i*ecart - ecart/2 - fil,ecart-fil*2,0]) 
    square([fil*2,fil*2]); 
  }

for (i = [1:nbled-1]) {
    translate([i*ecart-depth/2,0,0]) 
    square([depth,ecart/2]); 
  }  
 }
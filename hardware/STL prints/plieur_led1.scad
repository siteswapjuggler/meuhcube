ecart   = 4;
hauteur = 5;
epais   = 1;
profondeur = 20;
largeur = 16.5;


difference() {
    cube([largeur*1.4,profondeur,hauteur+epais/2]);
    translate([largeur/2-ecart/2,3,0]) cylinder(10,0.5,0.5);
    translate([largeur/2+ecart/2,3,0]) cylinder(10,0.5,0.5);
    translate([largeur/2+ecart/2-epais/2,3,hauteur]) cube([epais,profondeur,epais]);
    translate([largeur/2+ecart/2,3,hauteur]) 
    rotate([0,0,-45])
    translate([-epais/2,0,0])
    cube([epais,profondeur,epais]);
}

/*

pi    = 3.14;
anode = 25;
ecart = 16.5;
marge = 3;
rayon = (marge+anode-ecart)/pi;
diam  = 2*rayon;
echo(diam);

$fn = 100;

//negatif();
//positif();
//fil();

module negatif() {
    difference() {
        cube([ecart-marge+2*rayon+2,6,10]);
        translate([0,4,0])
        rotate([0,0,10])
        cube([ecart-marge+2*rayon+2,6,10]);
        positif();
        fil();
    }
}

module fil() {
    translate([0,0,5-epais/2])
    union() {
        translate([0,2-epais/2,0])
        cube([ecart-marge,epais,epais]);
        
        translate([rayon-marge+ecart,2-epais/2,0])
        difference() {
            cylinder(epais,rayon+epais/2,rayon+epais/2);
            cylinder(epais,rayon-epais/2,rayon-epais/2);
            translate([0,-ecart/2,0]) cube([ecart,ecart,ecart],center=true);
            }
            
       translate([ecart-marge+2*rayon-epais/2,0,0])
       cube([epais,2-epais/2,epais]);
    }
}

module positif(){
    difference() {
union() {
cube([ecart-marge+2*rayon-epais/8,2,10]);
translate([rayon-marge+ecart,2,0])
difference() {
    translate([0,-epais/2,0])
cylinder(10,rayon,rayon);
translate([0,-ecart/2,ecart/2]) cube([ecart,ecart,ecart],center=true);
}
}
fil();
}
}
*/
pliure  = 5;        // distance pour la pliure
fil     = 0.5;      // épaisseur des pattes
ecart   = 3;        // écart entre les pattes
anode   = 29.6;     // longueur de l'anode
cathode = 28.1;     // longueur de la cathode
espace  = 16.5;     // espace entre les leds
led     = 5.4;      // auteur de la led


//----------------------------------------------------------------------------------
// ICI C'EST LE PROGRAMME QUI GÈRE
//----------------------------------------------------------------------------------

//plieur1();

module plieur1() {
    difference() {
        // masse principale
        cube([anode,anode+2*ecart-pliure,pliure+fil]);
        
        // passage pattes verticale
        translate([ecart*2,ecart*2,0]) cube([fil*2,fil*2,10]);;
        translate([ecart*3,ecart*2,0]) cube([fil*2,fil*2,10]);;
        
        // guide pattes horizontales
        translate([ecart*3,ecart*2,pliure]) 
        cube([fil*2,anode-pliure,fil*2]);

        // guide pattes diagonale
        translate([ecart*3,ecart*2,pliure]) 
        rotate([0,0,-45])
        translate([-fil,fil,0])
        cube([fil*2,anode,fil*2]);
     }
 }
 
 module plieur2() {
    difference() {
        // masse principale
        cube([anode,anode+2*ecart-pliure,pliure+fil]);
        
        // passage pattes verticale
        translate([ecart*2,ecart*2,0]) cube([fil*2,fil*2,10]);;
        translate([ecart*3,ecart*2,0]) cube([fil*2,fil*2,10]);;
        
        // guide pattes horizontales
        translate([ecart*3,ecart*2,pliure]) 
        cube([fil*2,anode-pliure,fil*2]);

        // guide pattes diagonale
        translate([ecart*3,ecart*2,pliure]) 
        rotate([0,0,-45])
        translate([-fil,fil,0])
        cube([fil*2,anode,fil*2]);
        
        // coupe 
        cube([anode,2*ecart,pliure+fil]);
     }
 }
 

pi    = 3.14159265359;
marge = 3.13363;
rayon = (anode-marge-espace+led/2)/(pi-1);
reste =  espace - rayon - led/2;
epais = 10;

$fn = 100;

negatif();
//positif();
//fil();
 

module negatif() {
    difference() {
        translate([0,-marge-fil,-epais/2])
        cube([reste+2*rayon+4*fil,rayon+4*fil+marge,epais]);
        translate([0,-2*marge-fil,-epais/2])
        cube([reste+2*rayon,2*marge,epais]);
        translate([0,1,-epais/2])
        rotate([0,0,30])
        cube([anode,2*marge,epais]);
        positif();
        fil();
    }
}

module fil() {
    translate([0,-fil,-fil])
    union() {
        // partie principale
        cube([reste,fil*2,fil*2]);
        
        // partie arrondie
        translate([reste+rayon,0,0])
        difference() {
            cylinder(fil*2,rayon+fil,rayon+fil);    
            cylinder(fil*2,rayon-fil,rayon-fil);
            translate([-rayon-fil,-rayon-fil,0])
            cube([2*(rayon+fil),rayon+fil,2*fil]);
            }
       
            
       // marge pour accroche
       translate([reste+2*rayon-fil,-marge,0])
       cube([2*fil,marge,fil*2]);
    }
}

module positif(){
    difference() {
    translate([0,0,-epais/2])
    difference() {
        union() {
            translate([0,-marge-fil,0])
            cube([reste+rayon,marge+fil,epais]);
            translate([reste+rayon,-marge-2*fil,0])
            cube([rayon,marge+fil,epais]);
            translate([reste+rayon,-fil,0])
            cylinder(epais,rayon,rayon);
        }
        translate([0,-2.5*marge-fil,0])
        cube([reste+2*rayon,2*marge,epais]);
    }
    fil();
    }
}

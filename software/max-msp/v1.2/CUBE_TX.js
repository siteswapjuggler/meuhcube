inlets=1;
outlets=1;

var sop  = 0xE7;
var eop  = 0x7E;
var cmd  = 0x00;
var data = Array();

function send() {
	outlet(0,sop,cmd,data.length,data,eop);
	}
send.local=1;

//----------------------------------------------------------

function clear() {
	set_all(0);
	}

function set_all(v) {
	cmd = 0x00;
	data = [v & 0xFF];
	send();
	}


function set_pixel(x,y,z,v) {
	cmd = 0x01;
	data = [x & 0xFF,y & 0xFF,z & 0xFF,v & 0xFF];
	send();
	}

function set_line(p,c1,c2,v) {
	cmd = 0x02;
	data = [p & 0xFF,c1 & 0xFF,c2 & 0xFF,v & 0xFF];
	send();
	}

function set_plan(p,c,v) {
	cmd = 0x03;
	data = [p & 0xFF,c & 0xFF];
	if (arguments.length == 3) {
		data.push(v & 0xFF);
		}
	else if (arguments.length == 9) {
 		for (i=2;i<arguments.length;i++) {
			data.push(arguments[i] & 0xFF);
			}
		}		
	send();
	}

function set_cube() {
	cmd = 0x04;
	data.length = 0;
	for (i = 0; i < 49; i++) data.push(arguments[i] & 0xFF);
	send();
	}

//----------------------------------------------------------

function reverse(v) {
	cmd = 0x10;
	data.length = 0;
	send();
	}

function rev_pixel(x,y,z,v) {
	cmd = 0x11;
	data = [x & 0xFF,y & 0xFF,z & 0xFF,v & 0xFF];
	send();
	}

function rev_line(p,c1,c2,v) {
	cmd = 0x12;
	data = [p & 0xFF,c1 & 0xFF,c2 & 0xFF,v & 0xFF];
	send();
	}

function rev_plan(p,c,v) {
	cmd = 0x13;
	data = [p & 0xFF,c & 0xFF];
	if (arguments.length == 3) {
		data.push(v & 0xFF);
		}
	else if (arguments.length == 9) {
 		for (i=2;i<arguments.length;i++) {
			data.push(arguments[i] & 0xFF);
			}
		}		
	send();
	}

function rev_cube() {
	cmd = 0x14;
	data.length = 0;
	for (i = 0; i < 49; i++) data.push(arguments[i] & 0xFF);
	send();
	}

//----------------------------------------------------------

function move(p,o) {
	cmd = 0x21;
	data.length = 0;
	data.push(p & 0xFF);
	if (arguments.length == 2) {
		data.push(o & 0xFF);
		}
	send();
	}

function modu(p) {
	cmd = 0x22;
	data.length = 0;
	data.push(p & 0xFF);
	if (arguments.length == 2) {
		data.push(o & 0xFF);
		}
	send();
	}

/*
#define MOVE_XYZ    0x21          // bouger l'ensemble des pixels dans l'axe choisi (trouver un moyen mémotechnique pour que ce soit facile à retenir, comme pour les lignes
#define MODU_XYZ    0x22          // mouvement torique
#define ROTA_XYZ    0x23          // rotation du plan
#define MIRO_XYZ    0x24          // mirroir sur un axe
*/

//----------------------------------------------------------

function version() {
 	cmd = 0xE0;
	data.length = 0;
	send();
	}

function default() {
 	cmd = 0xE1;
	data.length = 0;
	send();
	}

function load() {
 	cmd = 0xE2;
	data.length = 0;
	send();
	}

function save() {
 	cmd = 0xE3;
	data.length = 0;
	send();
	}

//----------------------------------------------------------

function set_extclock(b) {
	cmd = 0xF0;
	data.length = 0;
	data.push(b!=0);
	send();
	}

function set_framerate(v) {
	cmd = 0xF1;
	data.length = 0;
	data.push(v&0xFF);
	send();
	}

function set_layerrate(v) {
	cmd = 0xF2;
	data.length = 0;
	data.push((v>>8)&0xFF);
	data.push(v&0xFF);
	send();
	}

//----------------------------------------------------------

function refresh() {
	cmd = 0xFF;
	data.length = 0;
	send();
	}




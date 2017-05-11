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

function set_plan() {
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


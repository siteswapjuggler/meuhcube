inlets=1;
outlets=1;

var sop = 0xE7; 
var eop = 0x7E;

var id    = 0;
var sum   = 0;
var len   = 0;
var cmd   = 0;
var cks   = 0;
var byte  = 0;
var datas = Array();

function analyse() {
	switch (cmd) {
		case 0x00 : break;
		}
	}
analyse.local=1;


function parser(c) {
	switch (byte) {
		case 0 : if (c == sop) {
					sum = 0;
					datas.length = 0;
					byte++;
					}
				 break;

		case 1 : id = c;
				 sum += c;
				 byte++;
				 break;

		case 2 : cmd = c;
				 sum += c;
				 byte++;
				 break;
		

		case 3 : len = c * 256;
				 sum += c;
				 byte++;
				 break;

		case 4 : len = len + c;
				 sum += c;
				 byte++;
				 break;
				
	 	default : if (byte < 5+len) {
				  	datas.push(c);
					sum += c;
					byte++;
					}
				  else if (byte == 5 + len) {
					cks = c;
					sum = sum % 256;
					byte++;
					}
				  else if (byte == 6 + len) {
					byte = 0;
					if ((c == eop)&&(sum == cks)) {
						analyse();
						}
					}
		}
}
parser.local=1;


function anything() {
	for (i=0;i<arguments.length;i++) {
		parser(arguments[i]);
		}
	}
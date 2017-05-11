inlets=1;
outlets=2;

var voxel = new JitterMatrix("voxel");
var data = new Array();

function jit_matrix() {
	data.length = 0;
	for (x=0;x<voxel.dim[0];x++) {
		val = 0;
		for (y=0;y<voxel.dim[1];y++) {
			bit = voxel.getcell(x,y)[0] > 127;
			val |= bit;
			if (y < voxel.dim[1]-1) { 
				val = val << 1;
				}
			}
		data.push(val);
		}
	outlet(0,"set_plan",0,0,data);
	outlet(1,"set_cube",data,data,data,data,data,data,data);
	}
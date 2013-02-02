// var c_tessellate = Module.cwrap('tessellate', 'void', ['number', 'number', 'number', 
//                                                        'number', 'number', 'number']);

// var ta = new Float64Array([0,0,1,5,2,0,-1,3,3,3]);
// var p = Module._malloc(ta.byteLength);
// for (var i=0; i<10; ++i) {
//     Module.setValue(p+i*8, ta[i], 'double');
// }

// var contours = Module._malloc(8);
// Module.setValue(contours, p, 'i32');
// Module.setValue(contours+4, p+80, 'i32');

// var coordinates_out = Module._malloc(4);
// var tris_out = Module._malloc(4);
// var nverts = Module._malloc(4);
// var ntris = Module._malloc(4);

// console.log(Module.getValue(nverts, 'i32'));
// console.log(Module.getValue(ntris, 'i32'));

// c_tessellate(coordinates_out, nverts, tris_out, ntris, contours, contours+8);

// console.log(Module.getValue(nverts, 'i32'));
// console.log(Module.getValue(ntris, 'i32'));

console.log(tessellate([[0,0,1,5,2,0,-1,3,3,3]]));

console.log(tessellate([[0,0,3,0,3,3,0,3],[1,1,1,2,2,2,2,1]]));

console.log(tessellate([[0,0,3,0,3,3,0,3],[1,1,2,1,2,2,1,2]]));

console.log(tessellate([[-2,3,-2,0,2,0,2,3],
                        [-1,2,-1,1,1,1,1,2]]));

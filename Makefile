all: tessellate.js

EMCC=~/code/emscripten/emcc

_tessellate.js: dict.c mesh.c render.c tess.c geom.c memalloc.c normal.c priorityq.c sweep.c tessmono.c tessellate.c
	${EMCC} -O2 -s EXPORTED_FUNCTIONS="['_tessellate', '_malloc', '_free']" dict.c mesh.c render.c tess.c geom.c memalloc.c normal.c priorityq.c sweep.c tessmono.c tessellate.c -o _tessellate.js
	gcc dict.c mesh.c render.c tess.c geom.c memalloc.c normal.c priorityq.c sweep.c tessmono.c tessellate.c main.c -o tessellate

tessellate.js: _tessellate.js _begin.js _end.js tessellate_api.js
	cat _begin.js _tessellate.js tessellate_api.js _end.js > tessellate.js



#ifndef MODE_H
#define MODE_H

const int MAX_VERTICES = 128;
const int LINE_WIDTH = 8;
const int VERTEX_RADIUS = 50;
const int HALF_RADIUS = VERTEX_RADIUS / 2;

enum Mode {
	basic,
	edge,
	del,
	color
};

#endif

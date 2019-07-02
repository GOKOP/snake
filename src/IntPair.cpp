#include "IntPair.hpp"

IntPair::IntPair() {
	x = 0;
	y = 0;
}

IntPair::IntPair(int new_x, int new_y) {
	x = new_x;
	y = new_y;
}

bool IntPair::operator==(IntPair other) {
	return (x==other.x && y==other.y);
}

bool IntPair::operator!=(IntPair other) {
	return (x!=other.x || y!=other.y);
}

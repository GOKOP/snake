#include "Fruit.hpp"

Fruit::Fruit(int new_x, int new_y, int val) {
	pos.x = new_x;
	pos.y = new_y;
	value = val;
}

Fruit::Fruit(IntPair new_pos, int val) {
	pos.x = new_pos.x;
	pos.y = new_pos.y;
	value = val;
}

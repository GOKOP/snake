#include "Fruit.hpp"

Fruit::Fruit(int new_x, int new_y, int val) {
	pos.first = new_x;
	pos.second = new_y;
	value = val;
}

Fruit::Fruit(std::pair<int, int> new_pos, int val) {
	pos.first = new_pos.first;
	pos.second = new_pos.second;
	value = val;
}

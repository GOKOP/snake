#pragma once

#include <utility>

class Fruit {
public:
	std::pair<int, int> pos;
	int value; // how much does if feed the snake

	Fruit(int new_x, int new_y, int val);
	Fruit(std::pair<int, int> new_pos, int val);
};

#ifndef FRUIT_HPP
#define FRUIT_HPP

#include "IntPair.hpp"

class Fruit {
public:
	IntPair pos;
	int value; // how much does if feed the snake

	Fruit(int new_x, int new_y, int val);
	Fruit(IntPair new_pos, int val);
};

#endif

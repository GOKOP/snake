#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <vector>
#include "IntPair.hpp"
#include "Enums.hpp"

class Snake {
	std::vector<IntPair> body; //head is the first element
	int how_fed; // how much body will grow
	Direction head_direction; // where is the head heading

	void move_head();

public:
	Snake(IntPair start_pos, Direction start_direction, int size);

	int getBodySize();
	IntPair getBodyPiecePos(int index);
	IntPair getHeadPos();
	Direction getDirection();
	int howFed();

	void feed(int amount);
	void advancePos();
	void turn(Direction new_direction);
};

#endif

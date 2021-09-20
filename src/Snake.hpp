#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <vector>
#include <utility>
#include "Enums.hpp"

class Snake {
	std::vector<std::pair<int, int>> body; //head is the first element
	int how_fed; // how much body will grow
	Direction head_direction; // where is the head heading

	void move_head();

public:
	Snake(std::pair<int, int> start_pos, Direction start_direction, int size);

	int getBodySize();
	std::pair<int, int> getBodyPiecePos(int index);
	std::pair<int, int> getHeadPos();
	Direction getDirection();
	int howFed();

	void feed(int amount);
	void advancePos();
	void turn(Direction new_direction);
};

#endif

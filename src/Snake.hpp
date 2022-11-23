#pragma once

#include <vector>
#include "Enums.hpp"
#include "Vector2i.hpp"

class Snake {
	std::vector<Vector2i> body; //head is the first element
	int how_fed; // how much body will grow
	Direction head_direction; // where is the head heading

	void move_head();

public:
	Snake(Vector2i start_pos, Direction start_direction, int size);

	int getBodySize();
	Vector2i getBodyPiecePos(int index);
	Vector2i getHeadPos();
	Direction getDirection();
	int howFed();

	void feed(int amount);
	void advancePos();
	void turn(Direction new_direction);
};

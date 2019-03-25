#ifndef BODYPIECE_HPP
#define BODYPIECE_HPP

#include "IntPair.hpp"

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class BodyPiece {
	IntPair pos;
	Direction direction;

public:
	BodyPiece(IntPair new_pos, Direction new_direction);

	IntPair get_pos();
	Direction get_direction();

	void move(Direction new_direction); //piece will take new_direction after moving
	void turn(Direction new_direction); //using for sth else than head will make stuff weird
};

#endif

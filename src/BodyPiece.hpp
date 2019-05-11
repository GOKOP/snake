#ifndef BODYPIECE_HPP
#define BODYPIECE_HPP

#include "IntPair.hpp"
#include "Enums.hpp"

class BodyPiece {
	IntPair pos;
	Direction direction;

public:
	BodyPiece(IntPair new_pos, Direction new_direction);

	IntPair getPos();
	Direction getDirection();

	void move(Direction new_direction); //piece will take new_direction after moving
	void turn(Direction new_direction); //using for sth else than head will make stuff weird
};

#endif

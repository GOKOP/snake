#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <vector>
#include "BodyPiece.hpp"

class Snake {
	std::vector<BodyPiece> body; //head is the first element
	bool fed;

public:
	Snake(IntPair start_pos, Direction start_direction);

	int getBodySize();
	IntPair getBodyPiecePos(int index);
	bool isFed();

	void feed();
	void advancePos();
	void turn(Direction new_direction);
};

#endif

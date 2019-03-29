#include "Snake.hpp"

Snake::Snake(IntPair start_pos, Direction start_direction) {
	body.push_back(BodyPiece(start_pos, start_direction));
	fed = true;
}

int Snake::getBodySize() {
	return body.size();
}

IntPair Snake::getBodyPiecePos(int index) {
	return body[index].getPos();	
}

bool Snake::isFed() {
	return fed;
}

void Snake::feed() {
	fed = true;
}

void Snake::advancePos() {
	for(int i=body.size()-1; i>=0; --i) {
		if(i==body.size()-1 && fed) {
			IntPair old_pos         = body[i].getPos();
			Direction old_direction = body[i].getDirection();
			body.push_back(BodyPiece(old_pos, old_direction));
			fed = false;
		}

		if(i==0)
			body[i].move(body[i].getDirection());
		else
			body[i].move(body[i-1].getDirection());
	}
}

void Snake::turn(Direction new_direction) {
	body[0].turn(new_direction);	
}

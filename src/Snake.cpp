#include "Snake.hpp"

Snake::Snake(IntPair start_pos, Direction start_direction) {
	body.push_back(BodyPiece(start_pos, start_direction));
	how_fed = 3;
}

int Snake::getBodySize() {
	return body.size();
}

IntPair Snake::getBodyPiecePos(int index) {
	return body[index].getPos();	
}

IntPair Snake::getHeadPos() {
	return body[0].getPos();
}

int Snake::howFed() {
	return how_fed;
}

void Snake::feed(int amount) {
	how_fed += amount;
}

void Snake::advancePos() {
	for(int i=body.size()-1; i>=0; --i) {
		if(i==body.size()-1 && how_fed>0) {
			IntPair old_pos         = body[i].getPos();
			Direction old_direction = body[i].getDirection();
			body.push_back(BodyPiece(old_pos, old_direction));
			how_fed -= 1;
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

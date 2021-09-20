#include "Snake.hpp"

Snake::Snake(std::pair<int, int> start_pos, Direction start_direction, int size) {
	body.push_back(start_pos);
	how_fed = size;
	head_direction = start_direction;
}

int Snake::getBodySize() {
	return body.size();
}

std::pair<int, int> Snake::getBodyPiecePos(int index) {
	return body[index];
}

std::pair<int, int> Snake::getHeadPos() {
	return body[0];
}

Direction Snake::getDirection() {
	return head_direction;
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
			body.push_back(body[i]);
			how_fed -= 1;
		}

		if(i==0)
			move_head();
		else
			body[i] = body[i-1];
	}
}

void Snake::turn(Direction new_direction) {
	head_direction = new_direction;
}

void Snake::move_head() {
	switch(head_direction) {
		case UP:
			body[0] = {body[0].first, body[0].second-1}; break;
		case DOWN:
			body[0] = {body[0].first, body[0].second+1}; break;
		case LEFT:
			body[0] = {body[0].first-1, body[0].second}; break;
		case RIGHT:
			body[0] = {body[0].first+1, body[0].second}; break;
		default:
			break;
	}
}


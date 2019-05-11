#include "FruitManager.hpp"
#include <random>
#include <algorithm>

void FruitManager::add(IntPair win_size) {
	int rand_x = rand()%(win_size.x-2)+1;
	int rand_y = rand()%(win_size.y-2)+1;

	bool conflicts = false;
	do {
		conflicts = false;
		for(int i=0; i<getFruitCount(); ++i) {
			if(fruits[i].pos == IntPair(rand_x, rand_y)) conflicts = true;
		}
		if(conflicts) {
			rand_x = rand()%(win_size.x-2)+1;
			rand_y = rand()%(win_size.y-2)+1;
		}
	} while(conflicts);

	fruits.push_back(Fruit(rand_x, rand_y, 1));
}

void FruitManager::remove(int index) {
	if(index>=0 && index<fruits.size()) fruits.erase(fruits.begin() + index);
}

std::vector<Fruit> FruitManager::getFruits() {
	return fruits;
}

Fruit FruitManager::getFruit(int index) {
	return fruits[index];
}

int FruitManager::getFruitCount() {
	return fruits.size();
}

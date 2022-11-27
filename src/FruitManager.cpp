#include "FruitManager.hpp"

FruitManager::FruitManager(int min_fruits): min_fruits(min_fruits) {}

void FruitManager::add(Vector2i win_size) {
	int rand_x = rand()%(win_size.x-2)+1;
	int rand_y = rand()%(win_size.y-2)+1;

	bool conflicts = false;
	do {
		conflicts = false;
		for(int i=0; i<getFruitCount(); ++i) {
			if(fruits[i].pos == Vector2i{rand_x, rand_y}) conflicts = true;
		}
		if(conflicts) {
			rand_x = rand()%(win_size.x-2)+1;
			rand_y = rand()%(win_size.y-2)+1;
		}
	} while(conflicts);

	fruits.push_back(Fruit{{rand_x, rand_y}, 1});
}

void FruitManager::remove(int index) {
	if(index>=0 && index<fruits.size()) fruits.erase(fruits.begin() + index);
}

const std::vector<Fruit>& FruitManager::getFruits() const {
	return fruits;
}

Fruit FruitManager::getFruit(int index) const {
	return fruits[index];
}

int FruitManager::getFruitCount() const {
	return fruits.size();
}

void FruitManager::setMinFruits(int new_min) {
	min_fruits = new_min;
}

int FruitManager::getMinFruits() const {
	return min_fruits;
}

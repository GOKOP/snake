#include "FruitManager.hpp"
#include <random>
#include <algorithm>

void FruitManager::add(IntPair win_size) {
	int rand_x = rand()%win_size.x;
	int rand_y = rand()%win_size.y;

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

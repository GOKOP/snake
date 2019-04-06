#include "FruitManager.hpp"
#include <random>
#include <algorithm>

void FruitManager::add(IntPair win_size, std::mt19937& gen) {
	std::uniform_int_distribution<int> rand_x(1, win_size.x-2);
	std::uniform_int_distribution<int> rand_y(1, win_size.y-2);

	fruits.push_back(Fruit(rand_x(gen), rand_y(gen), 1));
}

void FruitManager::remove(int index) {
	if(index>=0 && index<fruits.size()) fruits.erase(fruits.begin() + index);
}

std::vector<Fruit> FruitManager::getFruits() {
	return fruits;
}

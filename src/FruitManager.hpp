#pragma once

#include <vector>
#include <random>
#include <utility>

#include "Fruit.hpp"

class FruitManager {
	std::vector<Fruit> fruits;
	int min_fruits;

public:
	FruitManager(int new_min);
	std::vector<Fruit> getFruits();
	Fruit getFruit(int index);
	int getFruitCount();
	int getMinFruits();

	void add(std::pair<int, int> win_size);
	void remove(int index);
	void setMinFruits(int new_min);
};

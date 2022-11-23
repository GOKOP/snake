#pragma once

#include <vector>
#include <random>
#include <utility>

#include "Fruit.hpp"

class FruitManager {
	std::vector<Fruit> fruits;
	int min_fruits;

public:
	FruitManager(int min_fruits);

	const std::vector<Fruit>& getFruits() const;
	Fruit getFruit(int index) const;
	int getFruitCount() const;
	int getMinFruits() const;

	void add(std::pair<int, int> win_size);
	void remove(int index);
	void setMinFruits(int new_min);
};

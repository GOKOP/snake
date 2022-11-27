#pragma once

#include <string_view>
#include <optional>
#include "Vector2i.hpp"
#include "globals.hpp"

namespace default_config {
	const int min_fruits = 1;
	const Vector2i window_size = {40, 20};
	const int snake_length = 3;
	const int snake_delay = static_config::speed2_ms;
}

class CommandConfig {
	Vector2i window_size;
	int snake_delay;
	int min_fruits;
	int beg_length;
	bool is_ok;

	std::optional<int> stringToPositiveInt(std::string_view str) const;
	std::optional<Vector2i> readWinSize(std::string_view size_str) const;
	std::optional<int> readSnakeDelay(std::string_view delay_str) const;

public:

	CommandConfig(int argc, char* argv[]);

	Vector2i getWindowSize() const;
	int getSnakeDelay() const;
	int getMinFruits() const;
	int getBegLength() const;
	bool isOk() const;
};

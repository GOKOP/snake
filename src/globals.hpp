#pragma once

#include <string>
#include "Vector2i.hpp"

namespace static_config {
	const std::string version = "v1.1.2";

	const int speed1_ms = 150;
	const int speed2_ms = 100;
	const int speed3_ms = 50;
	const int speed4_ms = 30;

	const char snake_head = '@';
	const char snake_dead = 'X';
	const char snake_body = 'o';
	const char border = '#';
	const char fruit = '*';
}

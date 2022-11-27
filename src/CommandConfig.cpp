#include "CommandConfig.hpp"
#include "globals.hpp"
#include <unistd.h>
#include <charconv>

CommandConfig::CommandConfig(int argc, char* argv[]): is_ok{true} {

	std::optional<Vector2i> window_size_result = std::nullopt;
	std::optional<int> snake_delay_result = std::nullopt;
	std::optional<int> min_fruits_result = std::nullopt;
	std::optional<int> beg_length_result = std::nullopt;

	int opt;
	while( (opt = getopt(argc, argv, "hw:s:f:l:")) != -1 ) {
		switch(opt) {
			case 'w':
				if(optarg) window_size_result = readWinSize(optarg);
				else is_ok = false;
				break;
			case 's':
				if(optarg) snake_delay_result = readSnakeDelay(optarg);
				else is_ok = false;
				break;
			case 'f':
				if(optarg) min_fruits_result = readMinFruits(optarg);
				else is_ok = false;
				break;
			case 'l':
				if(optarg) beg_length_result = readBegLength(optarg);
				else is_ok = false;
				break;
			case 'h': 
			default: is_ok = false;
		}
	}

	window_size = window_size_result.value_or(default_config::window_size);
	snake_delay = snake_delay_result.value_or(default_config::snake_delay);
	min_fruits = min_fruits_result.value_or(default_config::min_fruits);
	beg_length = beg_length_result.value_or(default_config::snake_length);
}

std::optional<int> CommandConfig::stringToInt(std::string_view str) const {
	int value;

	const char* begin = str.data();
	const char* end = str.data() + str.size();

	auto result = std::from_chars(begin, end, value);
	if(result.ec != std::errc() || result.ptr != end) return {};
	else return value;
}

std::optional<Vector2i> CommandConfig::readWinSize(std::string_view size_str) const {
	auto x_pos = size_str.find('x');
	if(x_pos == std::string_view::npos || x_pos == size_str.size() - 1) return {};

	auto width_str = size_str.substr(0, x_pos);
	auto height_str = size_str.substr(x_pos + 1);

	auto width_result = stringToInt(width_str);
	auto height_result = stringToInt(height_str);

	if(!width_result.has_value() || !height_result.has_value()) return {};

	return Vector2i{width_result.value(), height_result.value()};
}

std::optional<int> CommandConfig::readSnakeDelay(std::string_view delay_str) const {
	int delay = 0;

	auto ms_pos = delay_str.find("ms");

	// if given miliseconds, set the delay directly
	if(ms_pos != std::string_view::npos && ms_pos == delay_str.size() - 2) {
		delay_str.remove_suffix(2);
		delay = stringToInt(delay_str).value_or(0);
	} // else set based on predefined speed
	else if(delay_str == "1") delay = static_config::speed1_ms;
	else if(delay_str == "2") delay = static_config::speed2_ms;
	else if(delay_str == "3") delay = static_config::speed3_ms;
	else if(delay_str == "4") delay = static_config::speed4_ms;

	if(delay <= 0) return {};
	return delay;
}

std::optional<int> CommandConfig::readMinFruits(std::string_view num_str) const {
	auto min_num_result = stringToInt(num_str);
	if(!min_num_result.has_value()) return {};
	int min_num = min_num_result.value();
	if(min_num <= 0) return {};
	return min_num;
}

std::optional<int> CommandConfig::readBegLength(std::string_view len_str) const {
	auto len_num_result = stringToInt(len_str);
	if(!len_num_result.has_value()) return {};
	int len_num = len_num_result.value();
	if(len_num <= 0) return {};
	return len_num;
}

Vector2i CommandConfig::getWindowSize() const {
	return window_size;
}

int CommandConfig::getSnakeDelay() const {
	return snake_delay;
}

int CommandConfig::getMinFruits() const {
	return min_fruits;
}

int CommandConfig::getBegLength() const {
	return beg_length;
}

bool CommandConfig::isOk() const {
	return is_ok;
}

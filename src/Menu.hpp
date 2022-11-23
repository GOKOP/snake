#pragma once

#include <vector>
#include "MenuOption.hpp"

class Menu {
	std::string header;
	std::string corner_text;
	std::vector<MenuOption> options;
	int selected_index;

public:
	
	Menu(std::string_view header, std::string_view corner_text);

	std::string getHeader();
	std::string getCornerText();
	int getOptionCount();
	int getSelection();
	MenuOption getOption(int index);

	void addOption(MenuOption option);
	void moveSelection(Direction dir);
};

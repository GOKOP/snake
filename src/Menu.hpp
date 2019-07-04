#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include "MenuOption.hpp"

class Menu {
	std::string header;
	std::string corner_text;
	std::vector<MenuOption> options;
	int selected_index;

public:
	
	Menu(std::string new_header, std::string new_corner="");

	std::string getHeader();
	std::string getCornerText();
	int getOptionCount();
	int getSelection();
	MenuOption getOption(int index);

	void addOption(MenuOption option);
	void moveSelection(Direction dir);
};

#endif

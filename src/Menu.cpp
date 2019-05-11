#include "Menu.hpp"

Menu::Menu(std::string new_header) {
	header = new_header;
	selected_index = 0;
}

std::string Menu::getHeader() {
	return header;
}

int Menu::getOptionCount() {
	return options.size();
}

int Menu::getSelection() {
	return selected_index;
}

MenuOption Menu::getOption(int index) {
	return options[index];
}

void Menu::addOption(MenuOption option) {
	options.push_back(option);
}

void Menu::moveSelection(Direction dir) {
	if(dir==RIGHT || dir==LEFT) return;

	if(dir==UP)        --selected_index;
	else if(dir==DOWN) ++selected_index;

	if(selected_index < 0)
		selected_index = options.size()-1;
	else if(selected_index >= options.size())
		selected_index = 0;
}

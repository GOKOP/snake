#include "ColorPair.hpp"

ColorPair::ColorPair(int num, int fground, int bground, bool is_bold) {
	init_pair(num, fground, bground);
	pair_num = num;
	bold = is_bold;
}

void ColorPair::enable(WINDOW* win) {
	wattron(win, COLOR_PAIR(pair_num));
	if(bold) wattron(win, A_BOLD);
}

void ColorPair::disable(WINDOW* win) {
	wattroff(win, COLOR_PAIR(pair_num));
	if(bold) wattroff(win, A_BOLD);
}

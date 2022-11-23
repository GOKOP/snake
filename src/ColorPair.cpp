#include "ColorPair.hpp"

ColorPair::ColorPair(int num, int fground, int bground, bool is_bold):
	pair_num(num),
	bold(is_bold)
{
	init_pair(num, fground, bground);
}

ColorPair::ColorPair(): pair_num(-1) {}

void ColorPair::enable(WINDOW* win) const {
	if(pair_num < 0) return;
	wattron(win, COLOR_PAIR(pair_num));
	if(bold) wattron(win, A_BOLD);
}

void ColorPair::disable(WINDOW* win) const {
	if(pair_num < 0) return;
	wattroff(win, COLOR_PAIR(pair_num));
	if(bold) wattroff(win, A_BOLD);
}

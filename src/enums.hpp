#pragma once

enum class GameState {
	None,
	MainMenu,
	Running,
	Lost,
	Quit,
	Canceled
};

enum class Direction {
	Up,
	Down,
	Left,
	Right
};

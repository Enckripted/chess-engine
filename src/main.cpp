#include <cassert>
#include <iostream>
#include <vector>

#include "game/board.hpp"
#include "game/game_result.hpp"
#include "move_generator.hpp"

#include "tui.hpp"

bool dogame = true;

int main()
{
	if (!dogame)
		return 0;

	// this will get a makeover at some point
	auto t = TerminalUi();
	t.StartGame();
	return 0;
}
#include "game/board.hpp"

class TerminalUi
{
private:
	Board gameBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	void PrintGameState();
	void PlayTurn();
	void PrintEndScreen();

public:
	void StartGame();
};
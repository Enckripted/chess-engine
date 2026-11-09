#include "board.hpp"

enum class GameResult
{
	InProgress,
	WinWhite,
	WinBlack,
	Draw,
};

class GameResultEvaluator
{
private:
	GameResultEvaluator() = delete;

public:
	// like everything else this is extremely slow right now and will be redone later
	static GameResult EvaluateBoard(Board &board);
};
#include <iostream>

#include "game/piece.hpp"
#include "game/move.hpp"
#include "game/board.hpp"

bool dogame = true;

Board gameBoard;

void playGame()
{
	bool whiteKing = false;
	bool blackKing = false;
	std::array<Piece, BOARD_SIZE> state = gameBoard.getState();
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (state[i].type() != King)
			continue;
		if (state[i].color() == White)
			whiteKing = true;
		if (state[i].color() == Black)
			blackKing = true;
	}
	if (!whiteKing)
	{
		std::cout << "Black wins!" << std::endl;
		return;
	}
	else if (!blackKing)
	{
		std::cout << "White wins!" << std::endl;
		return;
	}

	std::cout << gameBoard.ToString();
	std::cout << "\n\n"
			  << (gameBoard.getTurn() == White ? "White" : "Black") << " to move" << std::endl;
	std::cout << "Enter your move: (UCI format, 'exit' to stop)" << std::endl;
	std::string userMove;
	std::cin >> userMove;
	try
	{
		if (userMove == "exit")
			return;
		Move move = Move(userMove);
		gameBoard.MakeMove(move);
	}
	catch (const char *msg)
	{
	}
	playGame();
}

void startGame()
{
	gameBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	playGame();
}

int main()
{
	if (dogame)
	{
		startGame();
		return 0;
	}

	return 0;
}
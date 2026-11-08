#include <cassert>
#include <iostream>
#include <vector>

#include "game/board.hpp"

bool dogame = true;

Board gameBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

void playGame()
{
	bool whiteKing = false;
	bool blackKing = false;
	std::array<Piece, 64> state = gameBoard.state();
	for (int i = 0; i < 64; i++)
	{
		if (state[i].type() != PieceType::King)
			continue;
		if (state[i].color() == PieceColor::White)
			whiteKing = true;
		if (state[i].color() == PieceColor::Black)
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
			  << (gameBoard.current_turn() == PieceColor::White ? "White" : "Black") << " to move" << std::endl;
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
}
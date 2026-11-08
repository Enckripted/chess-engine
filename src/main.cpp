#include <cassert>
#include <iostream>
#include <vector>

#include "game/board.hpp"

bool dogame = false;

Board gameBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

void playGame()
{
	bool whiteKing = false;
	bool blackKing = false;
	std::array<Piece, 64> state = gameBoard.state();
	for (int i = 0; i < 8; i++)
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

	auto expect_check = [](const std::string &description, const std::string &fen)
	{
		std::cout << "Test: " << description << std::endl;
		Board board(fen);
		assert(board.CurrentlyInCheck());
	};

	expect_check("queen on diagonal", "8/8/8/8/7q/8/8/4K3 w - - 0 1");
	expect_check("rook on same file", "4r3/8/8/8/8/8/8/4K3 w - - 0 1");
	expect_check("bishop on diagonal", "8/8/8/8/8/2b5/8/4K3 w - - 0 1");
	expect_check("knight attack", "8/8/8/8/8/5n2/8/4K3 w - - 0 1");
	expect_check("pawn attack left", "8/8/8/8/8/8/3p4/4K3 w - - 0 1");
	expect_check("pawn attack right", "8/8/8/8/8/8/5p2/4K3 w - - 0 1");
	expect_check("black king in check by white queen", "4k3/8/8/7Q/8/8/8/8 b - - 0 1");
	expect_check("black king in check by white pawn", "4k3/3P4/8/8/8/8/8/8 b - - 0 1");

	std::cout << "All move generator tests passed." << std::endl;
	return 0;
}
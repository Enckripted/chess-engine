#include <cassert>
#include <iostream>
#include <vector>

#define private public
#define protected public
#include "move_generator.hpp"
#undef private
#undef protected

bool dogame = false;

Board gameBoard;

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

	{
		Board board("8/8/8/8/8/8/pk6/Rb6 w - - 0 1");
		MoveGenerator gen(board);
		std::vector<Move> dirMoves;
		std::cout << "Checking moveInDir" << std::endl;
		gen.MoveInDir(Space(0, 0), Space(0, 1), dirMoves);
		assert(dirMoves.size() == 1);
		assert(dirMoves[0] == Move(Space(0, 0), Space(0, 1)));
	}

	{
		Board board("8/8/8/8/8/8/8/P7 w - - 0 1");
		MoveGenerator gen(board);
		std::cout << "Checking pawn moves" << std::endl;
		auto moves = gen.PawnMovesFrom(Space(0, 0));
		assert(moves.size() == 1);
		assert(moves[0] == Move(Space(0, 0), Space(0, 1)));
	}

	{
		Board board("8/8/8/8/8/8/8/N7 w - - 0 1");
		MoveGenerator gen(board);
		std::cout << "Checking knight moves" << std::endl;
		auto moves = gen.KnightMovesFrom(Space(0, 0));
		assert(moves.size() == 2);
		assert(moves[0] == Move(Space(0, 0), Space(2, 1)) || moves[0] == Move(Space(0, 0), Space(1, 2)));
		assert(moves[1] == Move(Space(0, 0), Space(2, 1)) || moves[1] == Move(Space(0, 0), Space(1, 2)));
	}

	{
		Board board("8/8/8/8/8/8/8/K7 w - - 0 1");
		MoveGenerator gen(board);
		std::cout << "Checking king moves" << std::endl;
		auto moves = gen.KingMovesFrom(Space(0, 0));
		assert(moves.size() == 3);
		assert(moves[0] == Move(Space(0, 0), Space(1, 0)) || moves[0] == Move(Space(0, 0), Space(0, 1)) || moves[0] == Move(Space(0, 0), Space(1, 1)));
		assert(moves[1] == Move(Space(0, 0), Space(1, 0)) || moves[1] == Move(Space(0, 0), Space(0, 1)) || moves[1] == Move(Space(0, 0), Space(1, 1)));
		assert(moves[2] == Move(Space(0, 0), Space(1, 0)) || moves[2] == Move(Space(0, 0), Space(0, 1)) || moves[2] == Move(Space(0, 0), Space(1, 1)));
	}

	{
		Board board("8/8/8/8/8/8/pk6/Rb6 w - - 0 1");
		MoveGenerator gen(board);
		std::cout << "Checking rook moves" << std::endl;
		auto moves = gen.RookMovesFrom(Space(0, 0));
		assert(moves.size() == 2);
		assert(moves[0] == Move(Space(0, 0), Space(1, 0)));
		assert(moves[1] == Move(Space(0, 0), Space(0, 1)));
	}

	{
		Board board("8/8/8/8/8/8/1k6/B7 w - - 0 1");
		MoveGenerator gen(board);
		std::cout << "Checking bishop moves" << std::endl;
		auto moves = gen.BishopMovesFrom(Space(0, 0));
		assert(moves.size() == 1);
		assert(moves[0] == Move(Space(0, 0), Space(1, 1)));
	}

	{
		Board board("8/8/8/8/8/8/pk6/Rb6 w - - 0 1");
		MoveGenerator gen(board);
		std::cout << "Checking queen moves" << std::endl;
		auto moves = gen.QueenMovesFrom(Space(0, 0));
		assert(moves.size() == 3);
		assert(moves[0] == Move(Space(0, 0), Space(1, 0)));
		assert(moves[1] == Move(Space(0, 0), Space(0, 1)));
		assert(moves[2] == Move(Space(0, 0), Space(1, 1)));
	}

	std::cout << "All move generator tests passed." << std::endl;
	return 0;
}
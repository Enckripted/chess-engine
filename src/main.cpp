#include <iostream>

#include "game/piece.hpp"
#include "game/move.hpp"
#include "game/board.hpp"

int main()
{
	std::cout << "Hello, world!" << std::endl;
	std::cout << "Heller, world!" << std::endl;

	// basic piece testing
	Piece piece = Piece(King, Black);

	// primtive move testing
	Move awesomeMove = Move("e2e4");
	std::cout << "move from " << Move::indToSquare(awesomeMove.getStart()) << " to " << Move::indToSquare(awesomeMove.getEnd()) << std::endl;

	// primtive board testing
	std::string startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Board board = Board(startingFen);
	std::cout << "starting pos" << std::endl;
	std::cout << board.ToString() << std::endl;

	board.MakeMove(Move("e2e4"));
	std::cout << "after move" << std::endl;
	std::cout << board.ToString() << std::endl;

	board.MakeMove(Move("a1a3"));
	board.MakeMove(Move("a3e5"));
	board.MakeMove(Move("b1e4"));
	std::cout << "after move(s)" << std::endl;
	std::cout << board.ToString() << std::endl;
	return 0;
}
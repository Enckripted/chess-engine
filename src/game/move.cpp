#include <string>
#include <array>

#include "constants.hpp"
#include "move.hpp"

const std::array<char, BOARD_LEN> Move::ranks = {'1', '2', '3', '4', '5', '6', '7', '8'};
const std::array<char, BOARD_LEN> Move::files = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
const char Move::rankCharStart = '1';
const char Move::fileCharStart = 'a';

Move::Move() = default;

int Move::getStart()
{
	return start;
}

int Move::getEnd()
{
	return end;
}

int Move::squareToInd(std::string square)
{
	if (square.size() != 2)
		throw std::invalid_argument("square " + square + " should be len 2");

	int file = square[0] - Move::fileCharStart;
	int rank = square[1] - Move::rankCharStart;
	if (rank < 0 || rank >= BOARD_LEN || file < 0 || file >= BOARD_LEN)
		throw std::invalid_argument("square " + square + " is invalid notation");

	return rank * 8 + file;
}

std::string Move::indToSquare(int ind)
{
	int file = ind % BOARD_LEN;
	int rank = ind / BOARD_LEN;
	if (rank < 0 || rank >= BOARD_LEN || file < 0 || file >= BOARD_LEN)
		throw std::invalid_argument("ind " + std::to_string(ind) + " is invalid");

	return std::string{} + Move::files[file] + Move::ranks[rank];
}

Move::Move(std::string uciMove)
{
	if (uciMove.size() != 4)
		throw std::invalid_argument("uci move " + uciMove + " should be len 4");

	start = squareToInd(uciMove.substr(0, 2));
	end = squareToInd(uciMove.substr(2, 2));
}

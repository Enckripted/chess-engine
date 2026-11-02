#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include <string>
#include <array>

#include "constants.hpp"

class Move
{
private:
	int start;
	int end;

public:
	int getStart();
	int getEnd();

	// static helpers for dealing with UCI notation
	static const std::array<char, BOARD_LEN> ranks;
	static const std::array<char, BOARD_LEN> files;
	static const char rankCharStart;
	static const char fileCharStart;

	static int squareToInd(std::string square);
	static std::string indToSquare(int ind);

	// https://en.wikipedia.org/wiki/Universal_Chess_Interface#Design
	// TODO: maybe a version that uses algebraic notation?
	Move(std::string uciMove);
	Move();
};

#endif
#include "move.hpp"

#include <string>
#include <array>
#include <stdexcept>
#include <iostream>

Move::Move(std::string uciMove)
{
	if (uciMove.size() != 4)
		throw std::invalid_argument("uci move " + uciMove + " should be len 4");

	_start = Space(uciMove.substr(0, 2));
	_end = Space(uciMove.substr(2, 2));
}

std::string Move::ToString()
{
	return _start.ToStringNotation() + _end.ToStringNotation();
}

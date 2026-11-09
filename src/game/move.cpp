#include "move.hpp"

#include <string>
#include <array>
#include <stdexcept>
#include <iostream>

Move::Move(std::string uciMove)
{
	if (uciMove.size() < 4 || uciMove.size() > 5)
		throw std::invalid_argument("uci move " + uciMove + " should be len 4-5");

	_start = Space(uciMove.substr(0, 2));
	_end = Space(uciMove.substr(2, 2));
	if (uciMove.size() == 5)
	{
		Piece p = Piece::pieceFromChar(uciMove[4]);
		if (p.type() == PieceType::None || p.type() == PieceType::King)
			throw std::invalid_argument("attempted to set promote flag with char " + uciMove[4]);

		_promotion_flag = p.type();
	}
}

std::string Move::ToString()
{
	return _start.ToStringNotation() + _end.ToStringNotation();
}

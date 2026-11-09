#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <array>

#include "piece.hpp"
#include "space.hpp"

class Move
{
private:
	Space _start = Space();
	Space _end = Space();
	Piece _capture = Piece::None;
	Space _passant_target = Space();
	PieceType _promotion_flag = PieceType::None; // if this is not none, this should be a pawn trying to promote

public:
	inline Space start() const
	{
		return _start;
	};
	inline Space end() const
	{
		return _end;
	}
	inline Piece capture() const
	{
		return _capture;
	}
	inline Space passant_target() const
	{
		return _passant_target;
	}
	inline PieceType promotion_flag() const
	{
		return _promotion_flag;
	}

	inline void set_capture(Piece capture)
	{
		_capture = capture;
	}
	inline void set_passant_target(Space passant_target)
	{
		_passant_target = passant_target;
	}
	inline void set_promotion_flag(PieceType promotion_flag)
	{
		_promotion_flag = promotion_flag;
	}

	// https://en.wikipedia.org/wiki/Universal_Chess_Interface#Design
	// TODO: maybe a version that uses algebraic notation?
	Move(Space start, Space end, PieceType promotionFlag) : _start(start), _end(end), _promotion_flag(promotionFlag) {}
	Move(Space start, Space end) : _start(start), _end(end) {}
	Move(std::string uciMove);
	Move() = default;

	std::string ToString();

	inline bool operator==(const Move &other) const
	{
		return start() == other.start() && end() == other.end() && promotion_flag() == other.promotion_flag();
	}
};

#endif
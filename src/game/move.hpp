#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <array>
#include <cassert>

#include "piece.hpp"
#include "space.hpp"

enum class MoveFlag
{
	None,
	Promotion,
	Castling,
	Passant
};

enum class CastleFlag
{
	None,
	CastleQueenside,
	CastleKingside,
};

class Move
{
private:
	Space _start = Space();
	Space _end = Space();
	Piece _capture = Piece::None;
	Space _passant_target = Space();
	std::array<bool, 4> _castling_change = {};

	MoveFlag _flag = MoveFlag::None;
	PieceType _promotion_flag = PieceType::None; // if this is not none, this should be a pawn trying to promote
	CastleFlag _castling_flag = CastleFlag::None;

public:
	static const std::array<Move, 4> rook_castling_moves;
	static const std::array<Move, 4> king_castling_moves;

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
	inline CastleFlag castling_flag() const
	{
		return _castling_flag;
	}
	inline MoveFlag flag() const
	{
		return _flag;
	}
	inline auto castling_change() const
	{
		return _castling_change;
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
		_flag = MoveFlag::Promotion;
		_promotion_flag = promotion_flag;
	}
	inline void set_castle_flag(CastleFlag castle_flag)
	{
		_flag = MoveFlag::Castling;
		_castling_flag = castle_flag;
	}
	inline void set_passant_flag()
	{
		bool diagonal = abs(_start.file() - _end.file()) == 1 && abs(_start.rank() - _end.rank()) == 1;
		assert(diagonal);
		_flag = MoveFlag::Passant;
	}
	inline void set_castling_change(int i)
	{
		assert(i < 4);
		_castling_change[i] = true;
	}

	inline static int GetCastlingTypeInd(Move move)
	{
		// out of order cuz im dumb
		if (move.start() == Space(4, 0) && move.end() == Space(6, 0))
		{
			return 1;
		}
		else if (move.start() == Space(4, 0) && move.end() == Space(2, 0))
		{
			return 0;
		}
		else if (move.start() == Space(4, 7) && move.end() == Space(6, 7))
		{
			return 3;
		}
		else if (move.start() == Space(4, 7) && move.end() == Space(2, 7))
		{
			return 2;
		}
		return -1;
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

inline const std::array<Move, 4> Move::rook_castling_moves = {
	Move{{0, 0}, {3, 0}}, Move{{7, 0}, {5, 0}}, Move{{0, 7}, {3, 7}}, Move{{7, 7}, {5, 7}}};
inline const std::array<Move, 4> Move::king_castling_moves = {
	Move{{4, 0}, {2, 0}}, Move{{4, 0}, {6, 0}}, Move{{4, 7}, {2, 7}}, Move{{4, 7}, {6, 7}}};

#endif
#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <vector>
#include <array>

#include "game/move.hpp"
#include "game/board.hpp"

class MoveGenerator
{
private:
	MoveGenerator() = delete;

	inline static const std::array<Space, 4> rook_dirs{Space{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	inline static const std::array<Space, 4> bishop_dirs{Space{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

	inline static const std::array<Space, 8> knight_moves{
		Space{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}};
	inline static const std::array<Space, 8> king_moves{
		Space{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

	static void MoveInDir(Board &board, Space start, Space dir, std::vector<Move> &out);
	static std::vector<Move> FilterLegalMoves(Board &board, std::vector<Move> moves);

public:
	static std::vector<Move> PawnMovesFrom(Board &board, Space start);
	static std::vector<Move> RookMovesFrom(Board &board, Space start);
	static std::vector<Move> BishopMovesFrom(Board &board, Space start);
	static std::vector<Move> KnightMovesFrom(Board &board, Space start);
	static std::vector<Move> QueenMovesFrom(Board &board, Space start);
	static std::vector<Move> KingMovesFrom(Board &board, Space start);
	static std::vector<Move> MovesFrom(Board &board, Space start);

	static std::vector<Move> GenerateLegalMovesAt(Board &board, Space start);
	static std::vector<Move> GenerateLegalMoves(Board &board);
};

#endif
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

	inline static const std::array<PieceType, 4> pawn_promotions{PieceType::Knight, PieceType::Bishop, PieceType::Rook, PieceType::Queen};

	static void MoveInDir(Board &board, Space start, Space dir, std::vector<Move> &out);
	static void ConvertToPromotionMoves(Move move, std::vector<Move> &out);

public:
	static std::vector<Move> PawnMovesFrom(Board &board, Space start);
	static std::vector<Move> RookMovesFrom(Board &board, Space start);
	static std::vector<Move> BishopMovesFrom(Board &board, Space start);
	static std::vector<Move> KnightMovesFrom(Board &board, Space start);
	static std::vector<Move> QueenMovesFrom(Board &board, Space start);
	static std::vector<Move> KingMovesFrom(Board &board, Space start);
	static std::vector<Move> MovesFrom(Board &board, Space start);

	static void GenerateLegalMovesAt(Board &board, Space start, std::vector<Move> &out);
	static std::vector<Move> GenerateLegalMoves(Board &board);
};

#endif
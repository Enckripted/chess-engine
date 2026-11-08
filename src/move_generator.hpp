#include <vector>
#include <array>

#include "game/move.hpp"
#include "game/board.hpp"

class MoveGenerator
{
private:
	Board &board;

	inline static const std::array<Space, 4> rook_dirs{Space{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	inline static const std::array<Space, 4> bishop_dirs{Space{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

	inline static const std::array<Space, 8> knight_moves{
		Space{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}};
	inline static const std::array<Space, 8> king_moves{
		Space{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

	void MoveInDir(Space start, Space dir, std::vector<Move> &out);

	std::vector<Move> PawnMovesFrom(Space start);
	std::vector<Move> RookMovesFrom(Space start);
	std::vector<Move> BishopMovesFrom(Space start);
	std::vector<Move> KnightMovesFrom(Space start);
	std::vector<Move> QueenMovesFrom(Space start);
	std::vector<Move> KingMovesFrom(Space start);

	std::vector<Move> FilterLegalMoves(std::vector<Move> moves);

public:
	MoveGenerator(Board &board_) : board(board_) {}

	std::vector<Move> GenerateLegalMovesAt(Space start);
	std::vector<Move> GenerateLegalMoves();
};
#include "move_generator.hpp"

#include <iostream>

#include "game/board.hpp"

void MoveGenerator::MoveInDir(Board &board, Space start, Space dir, std::vector<Move> &out)
{
	PieceColor color = board.state()[start.ind()].color();

	Space cur = start;
	while (cur.is_valid())
	{
		cur += dir;
		if (!cur.is_valid())
			continue;

		// if its our piece we cant go further
		if (board.PieceOnSpace(cur).color() == color)
			break;

		// if the color is not ours we can go there
		out.push_back(Move(start, cur));
		// but if its a capture we have to stop there
		if (!board.IsSpaceEmpty(cur))
			break;
	}
}

std::vector<Move> MoveGenerator::PawnMovesFrom(Board &board, Space start)
{
	PieceColor color = board.state()[start.ind()].color();
	int dir = color == PieceColor::White ? 1 : -1;
	int initialPawnRank = color == PieceColor::White ? 2 : 6;

	Space forwardOne = start + Space(0, dir);
	Space forwardTwo = start + Space(0, dir * 2);
	Space leftDiagonal = start + Space(-1, 1);
	Space rightDiagonal = start + Space(1, -1);

	// TODO: at some point we will need to recgonize promotion
	// and generate legal moves with the different possible promotion flags
	std::vector<Move> moves;

	if (forwardOne.is_valid() && board.IsSpaceEmpty(forwardOne))
		moves.push_back(Move(start, forwardOne));
	if (forwardTwo.is_valid() && start.rank() == initialPawnRank && board.IsSpaceEmpty(forwardOne) && board.IsSpaceEmpty(forwardTwo))
		moves.push_back(Move(start, forwardTwo));
	// TODO: check for en passant (simple fix)
	if (leftDiagonal.is_valid() && !board.IsSpaceEmpty(leftDiagonal) && board.PieceOnSpace(leftDiagonal).color() != color)
		moves.push_back(Move(start, leftDiagonal));
	if (rightDiagonal.is_valid() && !board.IsSpaceEmpty(rightDiagonal) && board.PieceOnSpace(rightDiagonal).color() != color)
		moves.push_back(Move(start, rightDiagonal));

	return moves;
}

std::vector<Move> MoveGenerator::KnightMovesFrom(Board &board, Space start)
{
	PieceColor color = board.state()[start.ind()].color();

	std::vector<Move> moves;
	for (auto moveDir : knight_moves)
	{
		Space end = start + moveDir;
		if (moveDir.is_valid() && board.PieceOnSpace(end).color() != color)
			moves.push_back(Move(start, end));
	}
	return moves;
}

std::vector<Move> MoveGenerator::KingMovesFrom(Board &board, Space start)
{
	PieceColor color = board.state()[start.ind()].color();

	std::vector<Move> moves;
	for (auto moveDir : king_moves)
	{
		Space end = start + moveDir;
		if (moveDir.is_valid() && board.PieceOnSpace(end).color() != color)
			moves.push_back(Move(start, end));
	}
	return moves;
}

std::vector<Move> MoveGenerator::RookMovesFrom(Board &board, Space start)
{
	std::vector<Move> moves;
	for (auto dir : rook_dirs)
	{
		MoveInDir(board, start, dir, moves);
	}
	return moves;
}

std::vector<Move> MoveGenerator::BishopMovesFrom(Board &board, Space start)
{
	std::vector<Move> moves;
	for (auto dir : bishop_dirs)
	{
		MoveInDir(board, start, dir, moves);
	}
	return moves;
}

std::vector<Move> MoveGenerator::QueenMovesFrom(Board &board, Space start)
{
	std::vector<Move> moves;
	for (auto dir : rook_dirs)
	{
		MoveInDir(board, start, dir, moves);
	}
	for (auto dir : bishop_dirs)
	{
		MoveInDir(board, start, dir, moves);
	}
	return moves;
}
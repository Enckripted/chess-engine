#include "move_generator.hpp"

#include <iostream>

void MoveGenerator::MoveInDir(Space start, Space dir, std::vector<Move> &out)
{
	PieceColor color = board.state()[start.ind()].color();

	Space cur = start;
	std::cout << "starting at " + cur.ToStringNotation() << std::endl;
	std::cout << "dir " + dir.ToString() << std::endl;
	while (cur.is_valid())
	{
		cur += dir;
		if (cur.is_valid() && board.PieceOnSpace(cur).color() != color) // if the color is opposite, then we can capture
		{
			out.push_back(Move(start, cur));
			if (!board.IsSpaceEmpty(cur)) // but after capture we have to stop there
				break;
		}
	}
}

std::vector<Move> MoveGenerator::PawnMovesFrom(Space start)
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

	// FIXME check if the move is on the board
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

std::vector<Move> MoveGenerator::KnightMovesFrom(Space start)
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

std::vector<Move> MoveGenerator::KingMovesFrom(Space start)
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

std::vector<Move> MoveGenerator::RookMovesFrom(Space start)
{
	std::vector<Move> moves;
	for (auto dir : rook_dirs)
	{
		std::cout << "let's do " << dir.ToString() << std::endl;
	}
	for (auto dir : rook_dirs)
	{
		MoveInDir(start, dir, moves);
	}
	return moves;
}

std::vector<Move> MoveGenerator::BishopMovesFrom(Space start)
{
	std::vector<Move> moves;
	for (auto dir : bishop_dirs)
	{
		MoveInDir(start, dir, moves);
	}
	return moves;
}

std::vector<Move> MoveGenerator::QueenMovesFrom(Space start)
{
	std::vector<Move> moves;
	for (auto dir : rook_dirs)
	{
		MoveInDir(start, dir, moves);
	}
	for (auto dir : bishop_dirs)
	{
		MoveInDir(start, dir, moves);
	}
	return moves;
}
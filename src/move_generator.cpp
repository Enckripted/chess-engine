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

void MoveGenerator::ConvertToPromotionMoves(Move move, std::vector<Move> &out)
{
	for (auto promotionType : pawn_promotions)
		out.push_back(Move(move.start(), move.end(), promotionType));
}

std::vector<Move> MoveGenerator::PawnMovesFrom(Board &board, Space start)
{
	PieceColor color = board.state()[start.ind()].color();
	int dir = color == PieceColor::White ? 1 : -1;
	int initialPawnRank = color == PieceColor::White ? 1 : 6;
	int promotionPawnRank = color == PieceColor::White ? 7 : 0;

	Space forwardOne = start + Space(0, dir);
	Space forwardTwo = start + Space(0, dir * 2);
	Space leftDiagonal = start + Space(-1, 1);
	Space rightDiagonal = start + Space(1, 1);

	std::vector<Move> moves;

	if (forwardOne.is_valid() && board.IsSpaceEmpty(forwardOne))
	{
		if (forwardOne.rank() == promotionPawnRank)
			ConvertToPromotionMoves(Move(start, forwardOne), moves);
		else
			moves.push_back(Move(start, forwardOne));
	}

	if (forwardTwo.is_valid() && start.rank() == initialPawnRank && board.IsSpaceEmpty(forwardOne) && board.IsSpaceEmpty(forwardTwo))
	{
		Move move = Move(start, forwardTwo);
		move.set_passant_target(forwardOne);
		moves.push_back(move);
	}

	bool captureOnLeft = !board.IsSpaceEmpty(leftDiagonal) && board.PieceOnSpace(leftDiagonal).color() != color;
	if (leftDiagonal.is_valid() && (board.passant_target() == leftDiagonal || captureOnLeft))
	{
		if (leftDiagonal.rank() == promotionPawnRank)
			ConvertToPromotionMoves(Move(start, leftDiagonal), moves);
		else
			moves.push_back(Move(start, leftDiagonal));
	}

	bool captureOnRight = !board.IsSpaceEmpty(rightDiagonal) && board.PieceOnSpace(rightDiagonal).color() != color;
	if (rightDiagonal.is_valid() && (board.passant_target() == rightDiagonal || captureOnRight))
	{
		if (rightDiagonal.rank() == promotionPawnRank)
			ConvertToPromotionMoves(Move(start, rightDiagonal), moves);
		else
			moves.push_back(Move(start, rightDiagonal));
	}
	return moves;
}

std::vector<Move> MoveGenerator::KnightMovesFrom(Board &board, Space start)
{
	PieceColor color = board.state()[start.ind()].color();

	std::vector<Move> moves;
	for (auto moveDir : knight_moves)
	{
		Space end = start + moveDir;
		if (end.is_valid() && board.PieceOnSpace(end).color() != color)
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
		if (end.is_valid() && board.PieceOnSpace(end).color() != color)
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

std::vector<Move> MoveGenerator::MovesFrom(Board &board, Space start)
{
	switch (board.state()[start.ind()].type())
	{
	case PieceType::Pawn:
		return PawnMovesFrom(board, start);
	case PieceType::Knight:
		return KnightMovesFrom(board, start);
	case PieceType::Bishop:
		return BishopMovesFrom(board, start);
	case PieceType::Rook:
		return RookMovesFrom(board, start);
	case PieceType::Queen:
		return QueenMovesFrom(board, start);
	case PieceType::King:
		return KingMovesFrom(board, start);
	default:
		return {};
	}
}

void MoveGenerator::GenerateLegalMovesAt(Board &board, Space space, std::vector<Move> &out)
{
	if (board.PieceOnSpace(space).color() != board.current_turn())
		return;

	for (auto move : MovesFrom(board, space))
		if (board.IsMoveLegal(move))
			out.push_back(move);
}

std::vector<Move> MoveGenerator::GenerateLegalMoves(Board &board)
{
	std::vector<Move> moves;
	for (int i = 0; i < 64; i++)
	{
		GenerateLegalMovesAt(board, Space(i), moves);
	}
	return moves;
}
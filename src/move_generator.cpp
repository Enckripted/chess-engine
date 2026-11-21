#include "move_generator.hpp"

#include <iostream>

#include "game/board.hpp"

void MoveGenerator::MoveInDir(Board &board, Space start, Space dir, PieceColor color, std::vector<Move> &out)
{
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

std::vector<Move> MoveGenerator::PawnMovesFrom(Board &board, Space start, PieceColor color)
{
	int dir = color == PieceColor::White ? 1 : -1;
	int initialPawnRank = color == PieceColor::White ? 1 : 6;
	int promotionPawnRank = color == PieceColor::White ? 7 : 0;

	Space forwardOne = start + Space(0, dir);
	Space forwardTwo = start + Space(0, dir * 2);
	Space leftDiagonal = forwardOne + Space(-1, 0);
	Space rightDiagonal = forwardOne + Space(1, 0);

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

	bool captureOnLeft = leftDiagonal.is_valid() && !board.IsSpaceEmpty(leftDiagonal) && board.PieceOnSpace(leftDiagonal).color() != color;
	if (leftDiagonal.is_valid() && (board.passant_target() == leftDiagonal || captureOnLeft))
	{
		Move move = Move(start, leftDiagonal);
		if (leftDiagonal.rank() == promotionPawnRank)
		{
			ConvertToPromotionMoves(move, moves);
		}
		else if (board.passant_target() == leftDiagonal && !captureOnLeft)
		{
			move.set_passant_flag();
			moves.push_back(move);
		}
		else
		{
			moves.push_back(move);
		}
	}

	bool captureOnRight = rightDiagonal.is_valid() && !board.IsSpaceEmpty(rightDiagonal) && board.PieceOnSpace(rightDiagonal).color() != color;
	if (rightDiagonal.is_valid() && (board.passant_target() == rightDiagonal || captureOnRight))
	{
		Move move = Move(start, rightDiagonal);
		if (rightDiagonal.rank() == promotionPawnRank)
		{
			ConvertToPromotionMoves(move, moves);
		}
		else if (board.passant_target() == rightDiagonal && !captureOnRight)
		{
			move.set_passant_flag();
			moves.push_back(move);
		}
		else
		{
			moves.push_back(move);
		}
	}
	return moves;
}

std::vector<Move> MoveGenerator::KnightMovesFrom(Board &board, Space start, PieceColor color)
{
	std::vector<Move> moves;
	for (auto moveDir : knight_moves)
	{
		Space end = start + moveDir;
		if (end.is_valid() && board.PieceOnSpace(end).color() != color)
			moves.push_back(Move(start, end));
	}
	return moves;
}

std::vector<Move> MoveGenerator::KingMovesFrom(Board &board, Space start, PieceColor color)
{
	Piece king = board.PieceOnSpace(start);

	std::vector<Move> moves;
	for (auto moveDir : king_moves)
	{
		Space end = start + moveDir;
		if (end.is_valid() && board.PieceOnSpace(end).color() != color)
			moves.push_back(Move(start, end));
	}

	if (king.type() != PieceType::King || king.color() != color)
		return moves;

	int castleRank = color == PieceColor::White ? 0 : 7;
	int rightsIndex = color == PieceColor::White ? 0 : 2;
	auto castlingRights = board.castling_rights();

	bool queensideFree = board.IsSpaceEmpty(Space(1, castleRank)) && board.IsSpaceEmpty(Space(2, castleRank)) && board.IsSpaceEmpty(Space(3, castleRank));
	if (castlingRights[rightsIndex] && queensideFree)
	{
		Move castle(start, Space(2, castleRank));
		castle.set_castle_flag(CastleFlag::CastleQueenside);
		moves.push_back(castle);
	}

	bool kingsideFree = board.IsSpaceEmpty(Space(5, castleRank)) && board.IsSpaceEmpty(Space(6, castleRank));
	if (castlingRights[rightsIndex + 1] && kingsideFree)
	{
		Move castle(start, Space(6, castleRank));
		castle.set_castle_flag(CastleFlag::CastleKingside);
		moves.push_back(castle);
	}

	return moves;
}

std::vector<Move> MoveGenerator::RookMovesFrom(Board &board, Space start, PieceColor color)
{
	std::vector<Move> moves;
	for (auto dir : rook_dirs)
	{
		MoveInDir(board, start, dir, color, moves);
	}
	return moves;
}

std::vector<Move> MoveGenerator::BishopMovesFrom(Board &board, Space start, PieceColor color)
{
	std::vector<Move> moves;
	for (auto dir : bishop_dirs)
	{
		MoveInDir(board, start, dir, color, moves);
	}
	return moves;
}

std::vector<Move> MoveGenerator::QueenMovesFrom(Board &board, Space start, PieceColor color)
{
	std::vector<Move> moves;
	for (auto dir : rook_dirs)
	{
		MoveInDir(board, start, dir, color, moves);
	}
	for (auto dir : bishop_dirs)
	{
		MoveInDir(board, start, dir, color, moves);
	}
	return moves;
}

std::vector<Move> MoveGenerator::MovesFrom(Board &board, Space start, PieceColor color)
{
	switch (board.state()[start.ind()].type())
	{
	case PieceType::Pawn:
		return PawnMovesFrom(board, start, color);
	case PieceType::Knight:
		return KnightMovesFrom(board, start, color);
	case PieceType::Bishop:
		return BishopMovesFrom(board, start, color);
	case PieceType::Rook:
		return RookMovesFrom(board, start, color);
	case PieceType::Queen:
		return QueenMovesFrom(board, start, color);
	case PieceType::King:
		return KingMovesFrom(board, start, color);
	default:
		return {};
	}
}

std::vector<Move> MoveGenerator::PawnMovesFrom(Board &board, Space start)
{
	return PawnMovesFrom(board, start, board.PieceOnSpace(start).color());
}

std::vector<Move> MoveGenerator::KnightMovesFrom(Board &board, Space start)
{
	return KnightMovesFrom(board, start, board.PieceOnSpace(start).color());
}

std::vector<Move> MoveGenerator::BishopMovesFrom(Board &board, Space start)
{
	return BishopMovesFrom(board, start, board.PieceOnSpace(start).color());
}

std::vector<Move> MoveGenerator::RookMovesFrom(Board &board, Space start)
{
	return RookMovesFrom(board, start, board.PieceOnSpace(start).color());
}

std::vector<Move> MoveGenerator::QueenMovesFrom(Board &board, Space start)
{
	return QueenMovesFrom(board, start, board.PieceOnSpace(start).color());
}

std::vector<Move> MoveGenerator::KingMovesFrom(Board &board, Space start)
{
	return KingMovesFrom(board, start, board.PieceOnSpace(start).color());
}

std::vector<Move> MoveGenerator::MovesFrom(Board &board, Space start)
{
	return MovesFrom(board, start, board.PieceOnSpace(start).color());
}

void MoveGenerator::GenerateLegalMovesAt(Board &board, Space space, std::vector<Move> &out)
{
	if (board.PieceOnSpace(space).color() != board.current_turn())
		return;

	for (auto move : MovesFrom(board, space, board.current_turn()))
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
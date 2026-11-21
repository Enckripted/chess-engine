#include <cassert>
#include <iostream>
#include <vector>

#include "game/board.hpp"
#include "game/game_result.hpp"
#include "move_generator.hpp"
#include <argparse/argparse.hpp>

#include "tui.hpp"

bool dogame = true;
// gen ai castle tests
/*
namespace
{
	using Squares = std::array<std::array<char, 8>, 8>;

	struct CastleCase
	{
		bool white;
		bool kingside;
	};

	std::string MakeFen(CastleCase castle, int blockerFile = -1, int attackerFile = -1)
	{
		Squares squares;
		for (auto &rank : squares)
			rank.fill('.');

		const int homeRank = castle.white ? 0 : 7;
		const int opponentRank = castle.white ? 7 : 0;
		const char king = castle.white ? 'K' : 'k';
		const char rook = castle.white ? 'R' : 'r';
		const char enemyKing = castle.white ? 'k' : 'K';
		const char enemyRook = castle.white ? 'r' : 'R';
		const int rookFile = castle.kingside ? 7 : 0;
		const int enemyKingFile = attackerFile == 0 ? 7 : 0;

		squares[homeRank][4] = king;
		squares[homeRank][rookFile] = rook;
		squares[opponentRank][enemyKingFile] = enemyKing;

		if (blockerFile >= 0)
			squares[homeRank][blockerFile] = castle.white ? 'N' : 'n';
		if (attackerFile >= 0)
			squares[opponentRank][attackerFile] = enemyRook;

		std::string fen;
		for (int rank = 7; rank >= 0; --rank)
		{
			if (rank != 7)
				fen += '/';
			int emptyCount = 0;
			for (char square : squares[rank])
			{
				if (square == '.')
				{
					++emptyCount;
					continue;
				}
				if (emptyCount != 0)
				{
					fen += static_cast<char>('0' + emptyCount);
					emptyCount = 0;
				}
				fen += square;
			}
			if (emptyCount != 0)
				fen += static_cast<char>('0' + emptyCount);
		}

		const std::string right = castle.white
									  ? (castle.kingside ? "K" : "Q")
									  : (castle.kingside ? "k" : "q");
		fen += castle.white ? " w " : " b ";
		fen += right + " - 0 1";
		return fen;
	}

	std::string CastleMove(CastleCase castle)
	{
		const char rank = castle.white ? '1' : '8';
		return std::string("e") + rank + (castle.kingside ? "g" : "c") + rank;
	}

	bool Check(bool condition, const std::string &message)
	{
		if (!condition)
			std::cerr << message << '\n';
		return condition;
	}

	bool TestCastlingSucceeds(CastleCase castle)
	{
		Board board(MakeFen(castle));
		if (!Check(board.MakeMove(Move(CastleMove(castle))), "Castling move rejected: " + CastleMove(castle)))
			return false;

		const int rank = castle.white ? 0 : 7;
		const int kingFile = castle.kingside ? 6 : 2;
		const int rookFile = castle.kingside ? 5 : 3;
		const int rookStartFile = castle.kingside ? 7 : 0;
		const PieceColor color = castle.white ? PieceColor::White : PieceColor::Black;
		return Check(board.PieceOnSpace(Space(kingFile, rank)) == Piece(PieceType::King, color),
					 "King did not reach its castling square: " + CastleMove(castle)) &&
			   Check(board.PieceOnSpace(Space(rookFile, rank)) == Piece(PieceType::Rook, color),
					 "Rook did not reach its castling square: " + CastleMove(castle)) &&
			   Check(board.IsSpaceEmpty(Space(4, rank)) && board.IsSpaceEmpty(Space(rookStartFile, rank)),
					 "Castling origin squares were not cleared: " + CastleMove(castle));
	}

	bool TestBlockedCastling(CastleCase castle)
	{
		const std::vector<int> betweenFiles = castle.kingside
												  ? std::vector<int>{5, 6}
												  : std::vector<int>{1, 2, 3};
		for (int file : betweenFiles)
		{
			Board board(MakeFen(castle, file));
			if (!Check(!board.MakeMove(Move(CastleMove(castle))),
					   "Castling accepted with occupied path square on file " + std::to_string(file)))
				return false;
		}
		return true;
	}

	bool TestCastlingThroughCheck(CastleCase castle)
	{
		const std::vector<int> kingPathFiles = castle.kingside
												   ? std::vector<int>{4, 5, 6}
												   : std::vector<int>{4, 3, 2};
		for (int file : kingPathFiles)
		{
			Board board(MakeFen(castle, -1, file));
			if (!Check(!board.MakeMove(Move(CastleMove(castle))),
					   "Castling accepted through check on file " + std::to_string(file)))
				return false;
		}
		return true;
	}
}
*/
int main(int argc, char *argv[])
{
	/*
	const std::array<CastleCase, 4> castles{{
		{true, true},
		{true, false},
		{false, true},
		{false, false},
	}};

	for (CastleCase castle : castles)
	{
		std::cout << "running test" << std::endl;
		if (!TestCastlingSucceeds(castle) ||
			!TestBlockedCastling(castle) ||
			!TestCastlingThroughCheck(castle))
			return 1;
	}
	*/
	if (!dogame)
		return 0;

	// this will get a makeover at some point
	auto t = TerminalUi();
	t.StartGame();
	return 0;
}
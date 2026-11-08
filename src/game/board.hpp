#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <vector>
#include <string>

#include "piece.hpp"
#include "move.hpp"

/*
struct BoardMove
{
	Move move = Move("a1a1");
	Piece capture;
	std::array<bool, 4> castling_change{};
	// just so we can go back, en passant gets set by the board when going forward
	Space prev_passant;

	BoardMove(Move move_, Piece capture_, Space prev_passant_, std::array<bool, 4> castling_change_)
		: move(move_), capture(capture_), castling_change(castling_change_), prev_passant(prev_passant_) {}
};*/

class Board
{
private:
	std::array<Piece, 64> state_{};
	std::array<bool, 4> castling_rights_ = {1, 1, 1, 1};
	PieceColor current_turn_ = PieceColor::White;

	// if this is -1, then en passant cannot be done on this turn
	Space passant_target_ = Space(-1, -1);

	int halfmoves = 0;
	int fullmove = 1;

	std::vector<Move> past_moves;

	inline PieceColor OppositeColor(PieceColor color)
	{
		return color == PieceColor::White ? PieceColor::Black : PieceColor::White;
	}
	void ApplyMove(Move move); // applies a move without regard to legality
	void UnapplyMove();		   // reverses the last move made

public:
	// TODO: make these inline
	std::array<Piece, 64> state() const;
	std::array<bool, 4> castling_rights() const;
	PieceColor current_turn() const;
	Space passant_target() const;

	Board(std::string fen);

	bool CurrentlyInCheck(PieceColor kingColor);

	bool IsSpaceEmpty(Space space);
	Piece PieceOnSpace(Space space);

	bool IsBoardLegal();	  // check if the current turn's king will be in check
	bool MakeMove(Move move); // public facing move making, checks for illegal moves past just a future invalid board state

	std::string ToString();
};

#endif
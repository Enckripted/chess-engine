#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <vector>
#include <string>

#include "piece.hpp"
#include "move.hpp"

struct BoardMove
{
	Move move = Move("a1a1");
	Piece capture;
	std::array<bool, 4> castling_change{};
	// just so we can go back, en passant gets set by the board when going forward
	Space prev_passant;

	BoardMove(Move move_, Piece capture_, Space prev_passant_, std::array<bool, 4> castling_change_)
		: move(move_), capture(capture_), castling_change(castling_change_), prev_passant(prev_passant_) {}
};

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

	std::vector<BoardMove> past_moves;

public:
	std::array<Piece, 64> state() const;
	std::array<bool, 4> castling_rights() const;
	PieceColor current_turn() const;
	Space passant_target() const;

	// Board() : move_generator(MoveGenerator(*this)) {}
	Board(std::string fen);

	bool CurrentlyInCheck();

	bool IsSpaceEmpty(Space space);
	Piece PieceOnSpace(Space space);

	bool IsBoardLegalAfterMove(Move move); // check if the current turn's king will be in check
	void MakeMove(Move move);
	void UnmakeMove(); // reverses the last move made

	std::string ToString();
};

#endif
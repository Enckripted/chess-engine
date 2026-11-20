#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <vector>
#include <string>

#include "piece.hpp"
#include "move.hpp"

class Board
{
private:
	std::array<Piece, 64> _state{};
	std::array<bool, 4> _castling_rights = {1, 1, 1, 1};
	PieceColor _current_turn = PieceColor::White;

	// if this is -1, then en passant cannot be done on this turn
	Space _passant_target = Space(-1, -1);

	int halfmoves = 0;
	int fullmove = 1;

	std::vector<Move> past_moves;

	static inline const std::array<Space, 4> castling_rook_spaces{
		Space{0, 0}, {0, 7}, {7, 0}, {7, 7}};

	inline PieceColor OppositeColor(PieceColor color)
	{
		return color == PieceColor::White ? PieceColor::Black : PieceColor::White;
	}
	inline int _castle_rank()
	{
		return _current_turn == PieceColor::White ? 0 : 7;
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
	bool IsMoveCastling(Move move);

	bool IsSpaceEmpty(Space space);
	Piece PieceOnSpace(Space space);

	bool IsBoardLegal();		 // check if the current turn's king will be in check
	bool IsMoveLegal(Move move); // check if making this move would make IsBoardLegal false
	bool MakeMove(Move move);	 // public facing move making, checks for illegal moves past just a future invalid board state

	std::string ToString();
};

#endif
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
	std::array<bool, 4> _castling_rights{};
	PieceColor _current_turn = PieceColor::White;

	// if this is -1, then en passant cannot be done on this turn
	Space _passant_target = Space(-1, -1);

	int halfmoves = 0;
	int fullmove = 1;

	std::vector<Move> past_moves{};

	inline PieceColor OppositeColor(PieceColor color)
	{
		return color == PieceColor::White ? PieceColor::Black : PieceColor::White;
	}
	inline int castle_rank(PieceColor color)
	{
		return color == PieceColor::White ? 0 : 7;
	}
	inline void ApplyCastlingChange(std::array<bool, 4> change, bool forward)
	{
		for (int i = 0; i < 4; i++)
			if (change[i])
				_castling_rights[i] = !forward;
	}

	void UpdateMoveCastlingRights(Move &move);
	void MovePiece(Space const &start, Space const &end);
	void AddPiece(Piece piece, Space space);
	void RemovePieceAt(Space space);

public:
	// TODO: make these inline
	std::array<Piece, 64> state() const;
	std::array<bool, 4> castling_rights() const;
	PieceColor current_turn() const;
	Space passant_target() const;

	Board(std::string fen);

	bool SquareThreatenedByCheck(Space space, PieceColor color);
	bool CurrentlyInCheck(PieceColor kingColor);

	bool IsSpaceEmpty(Space space);
	Piece PieceOnSpace(Space space);

	bool IsBoardLegal();		 // check if the current turn's king will be in check
	bool IsMoveLegal(Move move); // check if making this move would make IsBoardLegal false
	void ApplyMove(Move move);	 // applies a move without regard to legality
	void UnapplyMove();			 // reverses the last move made
	bool MakeMove(Move move);	 // public facing move making, checks for illegal moves past just a future invalid board state
	void UnmakeMove();			 // public wrapper that reverses the last move made

	std::string ToString();
};

#endif
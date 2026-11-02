#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <vector>
#include <string>

#include "constants.hpp"
#include "piece.hpp"
#include "move.hpp"

const int BOARD_SIZE = BOARD_LEN * BOARD_LEN;

struct BoardMove
{
	Move move;
	Piece capture;
	std::array<bool, 4> castlingLoss{};
	// just so we can go back, en passant gets set by the board when going forward
	int prevEnPassant;

	BoardMove(Move mov, Piece cap, int prevPassant, std::array<bool, 4> castlingRightsChange);
};

class Board
{
private:
	// in theory every value here should be set by constructor but i figure
	// no harm in sensible defaults
	std::array<Piece, BOARD_SIZE> state{};
	std::array<bool, 4> castlingRights = {1, 1, 1, 1};
	PieceColor turn = White;
	// if this is -1, then en passant cannot be done on this turn
	int enPassantTarget = -1;
	int halfmoves = 0;
	int fullmove = 1;

	std::vector<BoardMove> pastMoves;

	Piece parseLetterToPiece(char c);

	bool LegalRookMove(int start, int end);
	bool LegalBishopMove(int start, int end);

public:
	// returns -1 if the space we're trying to move to is off the board
	static int moveFromPos(int pos, int x, int y);

	std::array<Piece, BOARD_LEN * BOARD_LEN> getState() const;
	std::array<bool, 4> getCastlingRights() const;
	PieceColor getTurn() const;
	int getEnPassantTarget() const;

	bool isSpaceEmpty(int ind) const;

	Board(std::string fen);
	Board() = default;
	bool IsMoveLegal(Move move);
	void MakeMove(Move move);
	// reverses the last move made
	void UnmakeMove();

	std::string ToString();
};

#endif
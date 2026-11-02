#include <string>
#include <iostream>
#include <sstream>

#include "board.hpp"

constexpr bool charIsInt(char c)
{
	return c >= '0' && c <= '9';
}

// move should maybe be a reference but i don't want to go through that hassle right now
BoardMove::BoardMove(Move mov, Piece cap, int prevPassant, std::array<bool, 4> castlingRightsChange)
{
	move = mov;
	capture = cap;
	prevEnPassant = prevPassant;
	castlingLoss = castlingRightsChange;
}

// TODO: maybe add a check for invalid boards?
Board::Board(std::string fen)
{
	std::vector<std::string> fields(6);
	int pos = 0;
	for (int i = 0; i < 6; i++)
	{
		int f = fen.find(" ", pos);
		if (f == fen.npos)
			f = fen.size();
		fields[i] = fen.substr(pos, f - pos);
		pos = f + 1;
	}

	// field 0: board state
	int ind = 56;
	std::cout << "field 0 " << fields[0] << std::endl;
	for (auto cur : fields[0])
	{
		if (cur == '/')
		{
			ind -= 16;
			continue;
		}
		else if (charIsInt(cur))
		{
			ind += cur - '0'; // converts char to number
			continue;
		}

		Piece piece = Piece::pieceFromChar(cur);
		state[ind] = piece;
		ind++;
	}

	// field 1: turn
	turn = fields[1] == "w" ? White : Black;

	// field 2: castling rights
	if (fields[2] != "-")
	{
		castlingRights[0] = fields[2].find('Q');
		castlingRights[1] = fields[2].find('K');
		castlingRights[2] = fields[2].find('q');
		castlingRights[3] = fields[2].find('k');
	}

	// field 3: en passant target
	if (fields[3] != "-")
	{
		Move::squareToInd(fields[3]);
	}

	// field 4: halfmove clock
	halfmoves = std::stoi(fields[4]);

	// field 5: fullmoves
	fullmove = std::stoi(fields[5]);
}

std::array<Piece, BOARD_LEN * BOARD_LEN> Board::getState() const
{
	return state;
}

std::array<bool, 4> Board::getCastlingRights() const
{
	return castlingRights;
}

PieceColor Board::getTurn() const
{
	return turn;
}

int Board::getEnPassantTarget() const
{
	return enPassantTarget;
}

bool Board::IsMoveLegal(Move move)
{
	int start = move.getStart();
	int end = move.getEnd();
	Piece movingPiece = state[move.getStart()];
	Piece capturedPiece = state[move.getEnd()];

	return true;

	if (movingPiece.type() == None || movingPiece.color() != turn)
		return false;

	if (movingPiece.type() == Pawn)
	{
		int direction = movingPiece.color() == White ? 1 : -1;

		// pawn can move forward one square
		if (start + direction * 8 == end && capturedPiece.type() == None)
			return true;

		// pawn can move forward two if its on starting rank
		int startRank = movingPiece.color() == White ? 2 : 6;
		int curRank = start / 8;
		if (start + direction * 16 == end && state[start + direction * 8].type() == None && capturedPiece.type() == None && startRank == curRank)
			return true;

		// alternatively, it can capture by moving diagonally, either capturing a piece or one square above en passant square
		bool movedUpDiagonal = abs(1) == 1;
		if (movedUpDiagonal && capturedPiece.type() != None)
		{
			return true;
		}
	}

	return true;
}

void Board::MakeMove(Move move)
{
	if (!IsMoveLegal(move))
	{
		std::cout << "made invalid move " + std::to_string(move.getStart()) + " to " + std::to_string(move.getEnd()) << std::endl;
		return;
	}

	// move already has bounds checking in constructor so no need to check that
	Piece movingPiece = state[move.getStart()];
	Piece capturedPiece = state[move.getEnd()];
	int prevEnPassant = enPassantTarget;

	// if we are moving a pawn two squares, update en passant target to its space, otherwise -1
	if (movingPiece.type() == Pawn && abs(move.getEnd() - move.getStart()) == 16)
	{
		enPassantTarget = move.getEnd();
	}
	else
	{
		enPassantTarget = -1;
	}

	// update castling rights on one side if a rook is captured or taken
	// if the king moves, lose castling rights on both sides
	// like stated before, this method does not check if the king should be able to castle
	// and assumes the check was already done
	int colorOffset = movingPiece.color() == White ? 0 : 2;
	std::array<bool, 4> castlingRightsChange{};
	if (castlingRights[colorOffset] && (movingPiece.type() == King || (movingPiece.type() == Rook && move.getStart() % 8 == 0)))
	{ // 1st file
		castlingRightsChange[colorOffset] = true;
	}
	if (castlingRights[colorOffset + 1] && (movingPiece.type() == King || (movingPiece.type() == Rook && move.getStart() % 8 == 7)))
	{ // 8th file
		castlingRightsChange[colorOffset + 1] = true;
	}

	for (int i = 0; i < 4; i++)
	{
		castlingRights[i] = castlingRights[i] && !castlingRightsChange[i];
	}

	// TODO: castle when the king is told to capture their own rook and they have rights

	state[move.getStart()] = Piece(None, White);
	state[move.getEnd()] = movingPiece;
	turn = turn == White ? Black : White;

	BoardMove boardMove = BoardMove(move, capturedPiece, prevEnPassant, castlingRightsChange);
	pastMoves.push_back(boardMove);
}

void Board::UnmakeMove()
{
}

std::string Board::ToString()
{
	std::ostringstream ss;
	for (int i = BOARD_LEN - 1; i > -1; i--)
	{
		ss << Move::ranks[i] << " ";
		for (int j = 0; j < BOARD_LEN; j++)
		{
			ss << Piece::charFromPiece(state[i * 8 + j]);
		}
		ss << '\n';
	}
	ss << "\n  ";
	for (int i = 0; i < BOARD_LEN; i++)
		ss << Move::files[i];
	return ss.str();
}
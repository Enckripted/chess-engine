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

bool Board::isSpaceEmpty(int ind) const
{
	return state[ind].type() == None;
}

int Board::moveFromPos(int start, int x, int y)
{
	int rank = start / 8 + y;
	int file = start % 8 + x;
	if (file < 0 || file >= BOARD_LEN || rank < 0 || rank > BOARD_LEN)
		return -1;
	return start + y * 8 + x;
}

// FIXME: these two share a lot of code, surely there's some way to bring them together
bool Board::LegalBishopMove(int start, int end)
{
	int x = (end % 8) - (start % 8);
	int y = (end / 8) - (start / 8);
	if (abs(x) != abs(y))
		return false;

	int dx = x / abs(x);
	int dy = y / abs(y);
	// std::cout << start << " " << end << std::endl;
	// std::cout << x << " " << y << " " << dx << " " << dy << std::endl;
	int pos = start;
	while (pos != end - dx - dy * 8)
	{
		pos += dx + dy * 8;
		// std::cout << "searched " << pos << std::endl;
		if (!isSpaceEmpty(pos))
			return false;
	}
	return true;
}

bool Board::LegalRookMove(int start, int end)
{
	int x = (end % 8) - (start % 8);
	int y = (end / 8) - (start / 8);
	if (x != 0 && y != 0)
		return false;

	int dx = x != 0 ? (x / abs(x)) : 0;
	int dy = y != 0 ? (y / abs(y)) : 0;
	int pos = start;

	while (pos != end - dx - dy * 8)
	{
		pos += dx + dy * 8;
		if (!isSpaceEmpty(pos))
			return false;
	}
	return true;
}

bool Board::IsMoveLegal(Move move)
{
	// start and end are guarenteed [0, 64)
	int start = move.getStart();
	int end = move.getEnd();
	Piece movingPiece = state[move.getStart()];
	Piece capturedPiece = state[move.getEnd()];

	if (movingPiece.type() == None)
		std::cout << "attempted to move piece of None" << std::endl;
	if (movingPiece.color() != turn)
		std::cout << "attempted to move on the wrong color turn" << std::endl;
	if (capturedPiece.color() == movingPiece.color())
		std::cout << "attempted to capture own piece" << std::endl;
	if (start == end || movingPiece.type() == None || movingPiece.color() != turn)
	{
		return false;
	}

	// TODO: implement promotions
	if (movingPiece.type() == Pawn)
	{
		int dir = movingPiece.color() == White ? 1 : -1;
		int upOne = moveFromPos(start, 0, dir);
		int upTwo = moveFromPos(start, 0, dir * 2);

		// pawn can move forward one square
		if (upOne == end && isSpaceEmpty(upOne))
			return true;

		// pawn can move forward two if its on starting rank
		int startRank = movingPiece.color() == White ? 1 : 6;
		int curRank = start / 8;
		if (upTwo == end && startRank == curRank && isSpaceEmpty(upOne) && isSpaceEmpty(upTwo))
			return true;

		// alternatively, it can capture by moving diagonally up one, either capturing a piece or one square above en passant square
		// TODO: implement en passant
		int leftDiag = moveFromPos(start, -1, dir);
		int rightDiag = moveFromPos(start, 1, dir);
		bool movedUpDiagonal = abs(upOne - end) == 1 && upOne;
		if ((leftDiag == end || rightDiag == end) && capturedPiece.type() != None)
		{
			return true;
		}
	}

	if (movingPiece.type() == Knight)
	{
		// check all combos of 1 and 2 with signs
		std::array<int, 8> dx = {-2, -2, -1, -1, 1, 1, 2, 2};
		std::array<int, 8> dy = {-1, 1, -2, 2, -2, 2, -1, 1};
		for (int i = 0; i < 8; i++)
		{
			if (end == moveFromPos(start, dx[i], dy[i]))
				return true;
		}
	}

	if (movingPiece.type() == Rook)
	{
		return LegalRookMove(start, end);
	}

	if (movingPiece.type() == Bishop)
	{
		return LegalBishopMove(start, end);
	}

	if (movingPiece.type() == Queen)
	{
		return LegalRookMove(start, end) || LegalBishopMove(start, end);
	}

	if (movingPiece.type() == King)
	{
		std::array<int, 8> dx = {-1, -1, -1, 0, 0, 1, 1, 1};
		std::array<int, 8> dy = {1, 0, -1, 1, -1, 1, 0, -1};
		for (int i = 0; i < 8; i++)
		{
			if (end == moveFromPos(start, dx[i], dy[i]))
				return true;
		}
	}

	return false;
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
	ss << "  ";
	for (int i = 0; i < BOARD_LEN; i++)
		ss << Move::files[i];
	return ss.str();
}
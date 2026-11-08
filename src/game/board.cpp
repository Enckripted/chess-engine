#include <string>
#include <iostream>
#include <sstream>

#include "board.hpp"
#include "../move_generator.hpp"

constexpr bool charIsInt(char c)
{
	return c >= '0' && c <= '9';
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
		state_[ind] = piece;
		ind++;
	}

	// field 1: turn
	current_turn_ = fields[1] == "w" ? PieceColor::White : PieceColor::Black;

	// field 2: castling rights
	if (fields[2] != "-")
	{
		castling_rights_[0] = fields[2].find('Q');
		castling_rights_[1] = fields[2].find('K');
		castling_rights_[2] = fields[2].find('q');
		castling_rights_[3] = fields[2].find('k');
	}

	// field 3: en passant target
	if (fields[3] != "-")
	{
		passant_target_ = Space(fields[3]);
	}

	// field 4: halfmove clock
	halfmoves = std::stoi(fields[4]);

	// field 5: fullmoves
	fullmove = std::stoi(fields[5]);
}

std::array<Piece, 64> Board::state() const
{
	return state_;
}

std::array<bool, 4> Board::castling_rights() const
{
	return castling_rights_;
}

PieceColor Board::current_turn() const
{
	return current_turn_;
}

Space Board::passant_target() const
{
	return passant_target_;
}

Piece Board::PieceOnSpace(Space space)
{
	return state_[space.ind()];
}

bool Board::IsSpaceEmpty(Space space)
{
	return PieceOnSpace(space).type() == PieceType::None;
}

// TODO: check if current turn king will be in check, otherwise then we're good
bool Board::IsBoardLegalAfterMove(Move move)
{
	return true;
}

// this is a very crappy way of doing this that is just so i can say i have it working
// later we'll do bitboards and all that stuff to make this check not unreasonably slow
bool Board::CurrentlyInCheck()
{
	// slow: find current turn king (we assume that this never fails)
	Space kingSpace = Space(0, 0);
	for (int i = 0; i < 64; i++)
	{
		if (state_[i].type() == PieceType::King && state_[i].color() == current_turn_)
		{
			kingSpace = Space(i);
		}
	}

	// super slow: generate all possible piece moves from that position (pawns capture in lane as bishops)
	// then go through all valid moves, if an enemy piece is on them of the corresponding type, then we are in check
	// we loop through the diagonals and straights seperately to make writing this a bit easier
	for (auto move : MoveGenerator::BishopMovesFrom(*this, kingSpace))
	{
		Piece attacker = state_[move.end().ind()];

		int attackingPawnDir = current_turn_ == PieceColor::Black ? 1 : -1;
		bool onAdjacentFile = abs(move.end().file() - kingSpace.file()) == 1;
		bool onPawnAttackSquare = onAdjacentFile && (kingSpace.rank() - move.end().rank()) == attackingPawnDir; // FIXME: complete me
		if (attacker.type() == PieceType::Pawn && onPawnAttackSquare)
			return true;
		if (attacker.type() == PieceType::Bishop || attacker.type() == PieceType::Queen)
			return true;
	}
	for (auto move : MoveGenerator::RookMovesFrom(*this, kingSpace))
	{
		Piece attacker = state_[move.end().ind()];

		if (attacker.type() == PieceType::Rook || attacker.type() == PieceType::Queen)
			return true;
	}
	for (auto move : MoveGenerator::KnightMovesFrom(*this, kingSpace))
	{
		Piece attacker = state_[move.end().ind()];

		if (attacker.type() == PieceType::Knight)
			return true;
	}

	return false;
}

// we are going to make the assumption that this move is at least psuedolegal
void Board::MakeMove(Move move)
{
	if (!IsBoardLegalAfterMove(move))
	{
		std::cout << "made invalid move " + move.start().ToStringNotation() + " to " + move.end().ToStringNotation() << std::endl;
		return;
	}

	// TODO:
	// Update castling rights when a castle is made
	// Update en passant whenever that happens
	// Handle promotions

	Piece capturedPiece = state_[move.end().ind()];
	auto boardMove = BoardMove(move, capturedPiece, passant_target_, std::array<bool, 4>{});

	state_[move.end().ind()] = state_[move.start().ind()];
	state_[move.start().ind()] = Piece(PieceType::None, PieceColor::None);
	past_moves.push_back(boardMove);
}

void Board::UnmakeMove()
{
}

std::string Board::ToString()
{
	std::ostringstream ss;
	for (int i = 7; i > -1; i--)
	{
		ss << Space::kRankChars[i] << " ";
		for (int j = 0; j < 8; j++)
		{
			ss << Piece::charFromPiece(state_[i * 8 + j]);
		}
		ss << '\n';
	}
	ss << "  ";
	for (int i = 0; i < 8; i++)
		ss << Space::kFileChars[i];
	return ss.str();
}
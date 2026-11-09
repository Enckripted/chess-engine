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

// this is a very crappy way of doing this that is just so i can say i have it working
// later we'll do bitboards and all that stuff to make this check not unreasonably slow
bool Board::CurrentlyInCheck(PieceColor kingColor)
{
	// slow: find current turn king (we assume that this never fails)
	Space kingSpace = Space(0, 0);
	for (int i = 0; i < 64; i++)
	{
		if (state_[i].type() == PieceType::King && state_[i].color() == kingColor)
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
		bool onPawnAttackSquare = onAdjacentFile && (kingSpace.rank() - move.end().rank()) == attackingPawnDir;
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

// this function makes the assumption that the given move is psuedolegal
void Board::ApplyMove(Move move)
{
	// TODO:
	// Update castling rights when a castle is made
	// Update en passant whenever that happens
	// Handle promotions

	Piece capturedPiece = state_[move.end().ind()];
	move.set_capture(capturedPiece);

	state_[move.end().ind()] = state_[move.start().ind()];
	state_[move.start().ind()] = Piece::None;
	passant_target_ = move.passant_target();

	if (move.promotion_flag() != PieceType::None)
	{
		state_[move.end().ind()] = Piece(move.promotion_flag(), current_turn_);
	}

	past_moves.push_back(move);
	current_turn_ = OppositeColor(current_turn_);
}

void Board::UnapplyMove()
{
	Move lastMove = past_moves.back();

	// default case
	state_[lastMove.start().ind()] = state_[lastMove.end().ind()]; // move the piece back to start
	state_[lastMove.end().ind()] = lastMove.capture();			   // add the captured piece back (if any)
	if (past_moves.size() > 1)									   // revert back to previous passant target
		passant_target_ = past_moves[past_moves.size() - 2].passant_target();
	else
		passant_target_ = Space();

	// if it was promoted, we have to revert it back to a pawn
	if (lastMove.promotion_flag() != PieceType::None)
	{
		state_[lastMove.start().ind()] = Piece(PieceType::Pawn, current_turn_);
	}

	past_moves.pop_back();
	current_turn_ = OppositeColor(current_turn_);
}

// TODO: check if current turn king will be in check, otherwise then we're good
bool Board::IsBoardLegal()
{
	if (CurrentlyInCheck(OppositeColor(current_turn_)))
	{
		// std::cout << "Cannot make move, currently in check" << std::endl;
		return false;
	}
	return true;
}

bool Board::IsMoveLegal(Move move)
{
	ApplyMove(move);
	if (!IsBoardLegal())
	{
		// std::cout << "made invalid move " + move.start().ToStringNotation() + " to " + move.end().ToStringNotation() << std::endl;
		UnapplyMove();
		return false;
	}
	UnapplyMove();
	return true;
}

// assumes this move is pseudolegal
bool Board::MakeMove(Move move)
{
	// basic sanity checks
	if (move.start().is_valid() && move.end().is_valid() && state_[move.start().ind()].color() != current_turn_ || state_[move.end().ind()].color() == current_turn_)
	{
		return false;
	}

	// std::cout << "basic sanity checks " << std::endl;
	bool psuedoLegal = false;
	for (auto psuedoMove : MoveGenerator::MovesFrom(*this, move.start()))
	{
		// std::cout << "psuedo legal " << psuedoMove.ToString() << std::endl;
		if (psuedoMove == move)
		{
			psuedoLegal = true;
			// IMPORTANT: this psuedo move has extra information encoded in, saving us having to write it later in this function
			move = psuedoMove;
			break;
		}
	}
	if (!psuedoLegal)
		return false;

	// std::cout << "psuedo legal pass" << std::endl;
	if (IsMoveLegal(move))
	{
		ApplyMove(move);
		return true;
	}
	return false;
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
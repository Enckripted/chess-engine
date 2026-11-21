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
	std::vector<std::string> fields;
	int pos = 0;
	while (fen.find(" ", pos) != std::string::npos)
	{
		int f = fen.find(" ", pos);
		if (f == fen.npos)
			f = fen.size();
		fields.push_back(fen.substr(pos, f - pos));
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
		_state[ind] = piece;
		ind++;
	}

	// field 1: turn
	_current_turn = fields[1] == "w" ? PieceColor::White : PieceColor::Black;

	// field 2: castling rights
	if (fields[2] != "-")
	{
		_castling_rights[0] = fields[2].find('Q') != std::string::npos;
		_castling_rights[1] = fields[2].find('K') != std::string::npos;
		_castling_rights[2] = fields[2].find('q') != std::string::npos;
		_castling_rights[3] = fields[2].find('k') != std::string::npos;
	}

	// field 3: en passant target
	if (fields.size() >= 4 && fields[3] != "-")
	{
		_passant_target = Space(fields[3]);
	}

	// field 4: halfmove clock
	if (fields.size() >= 5)
		halfmoves = std::stoi(fields[4]);

	// field 5: fullmoves
	if (fields.size() >= 6)
		fullmove = std::stoi(fields[5]);
}

std::array<Piece, 64> Board::state() const
{
	return _state;
}

std::array<bool, 4> Board::castling_rights() const
{
	return _castling_rights;
}

PieceColor Board::current_turn() const
{
	return _current_turn;
}

Space Board::passant_target() const
{
	return _passant_target;
}

Piece Board::PieceOnSpace(Space space)
{
	return _state[space.ind()];
}

bool Board::IsSpaceEmpty(Space space)
{
	return PieceOnSpace(space).type() == PieceType::None;
}

// this is a very crappy way of doing this that is just so i can say i have it working
// later we'll do bitboards and all that stuff to make this check not unreasonably slow
bool Board::SquareThreatenedByCheck(Space space, PieceColor color)
{
	// super slow: generate all possible piece moves from that position (pawns capture in lane as bishops)
	// then go through all valid moves, if an enemy piece is on them of the corresponding type, then we are in check
	// we loop through the diagonals and straights seperately to make writing this a bit easier

	for (auto move : MoveGenerator::BishopMovesFrom(*this, space, color))
	{
		Piece attacker = _state[move.end().ind()];

		int attackingPawnDir = color == PieceColor::Black ? 1 : -1;
		bool onAdjacentFile = abs(move.end().file() - space.file()) == 1;
		bool onPawnAttackSquare = onAdjacentFile && (space.rank() - move.end().rank()) == attackingPawnDir;
		if (attacker.type() == PieceType::Pawn && onPawnAttackSquare)
			return true;
		if (attacker.type() == PieceType::Bishop || attacker.type() == PieceType::Queen)
			return true;
	}
	for (auto move : MoveGenerator::RookMovesFrom(*this, space, color))
	{
		Piece attacker = _state[move.end().ind()];

		if (attacker.type() == PieceType::Rook || attacker.type() == PieceType::Queen)
			return true;
	}
	for (auto move : MoveGenerator::KnightMovesFrom(*this, space, color))
	{
		Piece attacker = _state[move.end().ind()];

		if (attacker.type() == PieceType::Knight)
			return true;
	}
	return false;
}

bool Board::CurrentlyInCheck(PieceColor kingColor)
{
	// slow: find current turn king (we assume that this never fails)
	Space kingSpace = Space(0, 0);
	for (int i = 0; i < 64; i++)
	{
		if (_state[i].type() == PieceType::King && _state[i].color() == kingColor)
		{
			kingSpace = Space(i);
		}
	}
	return SquareThreatenedByCheck(kingSpace, kingColor);
}

void Board::UpdateMoveCastlingRights(Move &move)
{
	int castleInd = _current_turn == PieceColor::White ? 0 : 2;
	int castleRank = castle_rank(_current_turn);
	int oppCastleInd = (castleInd + 2) % 4;
	int oppCastleRank = castle_rank(OppositeColor(_current_turn));

	// can't castle again after castling or moving king
	if (move.castling_flag() != CastleFlag::None || move.start() == Space(4, castleRank))
	{
		move.set_castling_change(castleInd);
		move.set_castling_change(castleInd + 1);
	}

	// can't castle on a side that their rook moves
	if (_castling_rights[castleInd] && move.start() == Space(0, castleRank))
	{
		move.set_castling_change(castleInd);
	}
	if (_castling_rights[castleInd + 1] && move.start() == Space(7, castleRank))
	{
		move.set_castling_change(castleInd + 1);
	}

	// if an opponents piece is taken, remove castling rights for the rook they lost
	if (_castling_rights[oppCastleInd] && move.end() == Space(0, oppCastleRank))
	{
		move.set_castling_change(oppCastleInd);
	}
	if (_castling_rights[oppCastleInd + 1] && move.end() == Space(7, oppCastleRank))
	{
		move.set_castling_change(oppCastleInd + 1);
	}
}

void Board::MovePiece(Space const &start, Space const &end)
{
	_state[end.ind()] = _state[start.ind()];
	_state[start.ind()] = Piece::None;
}

// this function makes the assumption that the given move is psuedolegal
void Board::ApplyMove(Move move)
{
	Piece capturedPiece = _state[move.end().ind()];
	move.set_capture(capturedPiece);

	if (move.castling_flag() == CastleFlag::None)
	{
		MovePiece(move.start(), move.end());
	}
	else
	{
		assert(Move::GetCastlingTypeInd(move) != -1);
		Move rookMove = Move::rook_castling_moves[Move::GetCastlingTypeInd(move)];
		Move kingMove = Move::king_castling_moves[Move::GetCastlingTypeInd(move)];
		MovePiece(rookMove.start(), rookMove.end());
		MovePiece(kingMove.start(), kingMove.end());
	}
	_passant_target = move.passant_target();

	if (move.promotion_flag() != PieceType::None)
	{
		_state[move.end().ind()] = Piece(move.promotion_flag(), _current_turn);
	}

	UpdateMoveCastlingRights(move);
	ApplyCastlingChange(move.castling_change(), true);

	past_moves.push_back(move);
	_current_turn = OppositeColor(_current_turn);
}

void Board::UnapplyMove()
{
	Move move = past_moves.back();

	// default case
	if (move.castling_flag() == CastleFlag::None)
	{
		MovePiece(move.end(), move.start());
		_state[move.end().ind()] = move.capture(); // add the captured piece back (if any)
	}
	else
	{
		assert(Move::GetCastlingTypeInd(move) != -1);
		// there is no need to put pieces back like above because these spaces should not be occupied anyways
		Move rookMove = Move::rook_castling_moves[Move::GetCastlingTypeInd(move)];
		Move kingMove = Move::king_castling_moves[Move::GetCastlingTypeInd(move)];
		MovePiece(rookMove.end(), rookMove.start());
		MovePiece(kingMove.end(), kingMove.start());
	}

	if (past_moves.size() > 1) // revert back to previous passant target
		_passant_target = past_moves[past_moves.size() - 2].passant_target();
	else
		_passant_target = Space();

	// if it was promoted, we have to revert it back to a pawn (from the opposite color since it was their turn then)
	if (move.promotion_flag() != PieceType::None)
	{
		_state[move.start().ind()] = Piece(PieceType::Pawn, OppositeColor(_current_turn));
	}

	ApplyCastlingChange(move.castling_change(), false);

	past_moves.pop_back();
	_current_turn = OppositeColor(_current_turn);
}

void Board::UnmakeMove()
{
	UnapplyMove();
}

bool Board::IsBoardLegal()
{
	if (CurrentlyInCheck(OppositeColor(_current_turn)))
	{
		// std::cout << "Cannot make move, currently in check" << std::endl;
		return false;
	}
	return true;
}

bool Board::IsMoveLegal(Move move)
{
	if (move.castling_flag() != CastleFlag::None)
	{
		if (Move::GetCastlingTypeInd(move) == -1)
		{
			std::cout << ToString() << std::endl;
			std::cout << move.ToString() << std::endl;
		}
		assert(Move::GetCastlingTypeInd(move) != -1); // temporarily for debugging

		Move kingMove = Move::king_castling_moves[Move::GetCastlingTypeInd(move)];
		int diff = kingMove.end().file() - kingMove.start().file();
		int dir = abs(diff) / diff;

		// castling rules say that we can't move out, through, or into check
		// so we just need to look for checks on the start square and second square in the movement
		Space cur = kingMove.start();
		Space next = cur + Space(dir, 0);

		bool threatened = SquareThreatenedByCheck(cur, _current_turn) || SquareThreatenedByCheck(next, _current_turn);
		if (threatened)
		{
			return false;
		}
	}

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
	if (move.start().is_valid() && move.end().is_valid() && _state[move.start().ind()].color() != _current_turn || _state[move.end().ind()].color() == _current_turn)
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
			ss << Piece::charFromPiece(_state[i * 8 + j]) << " ";
		}
		ss << '\n';
	}
	ss << "  ";
	for (int i = 0; i < 8; i++)
		ss << Space::kFileChars[i] << " ";
	return ss.str();
}
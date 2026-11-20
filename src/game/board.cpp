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
		_state[ind] = piece;
		ind++;
	}

	// field 1: turn
	_current_turn = fields[1] == "w" ? PieceColor::White : PieceColor::Black;

	// field 2: castling rights
	if (fields[2] != "-")
	{
		_castling_rights[0] = fields[2].find('Q');
		_castling_rights[1] = fields[2].find('K');
		_castling_rights[2] = fields[2].find('q');
		_castling_rights[3] = fields[2].find('k');
	}

	// field 3: en passant target
	if (fields[3] != "-")
	{
		_passant_target = Space(fields[3]);
	}

	// field 4: halfmove clock
	halfmoves = std::stoi(fields[4]);

	// field 5: fullmoves
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

	// super slow: generate all possible piece moves from that position (pawns capture in lane as bishops)
	// then go through all valid moves, if an enemy piece is on them of the corresponding type, then we are in check
	// we loop through the diagonals and straights seperately to make writing this a bit easier
	for (auto move : MoveGenerator::BishopMovesFrom(*this, kingSpace))
	{
		Piece attacker = _state[move.end().ind()];

		int attackingPawnDir = _current_turn == PieceColor::Black ? 1 : -1;
		bool onAdjacentFile = abs(move.end().file() - kingSpace.file()) == 1;
		bool onPawnAttackSquare = onAdjacentFile && (kingSpace.rank() - move.end().rank()) == attackingPawnDir;
		if (attacker.type() == PieceType::Pawn && onPawnAttackSquare)
			return true;
		if (attacker.type() == PieceType::Bishop || attacker.type() == PieceType::Queen)
			return true;
	}
	for (auto move : MoveGenerator::RookMovesFrom(*this, kingSpace))
	{
		Piece attacker = _state[move.end().ind()];

		if (attacker.type() == PieceType::Rook || attacker.type() == PieceType::Queen)
			return true;
	}
	for (auto move : MoveGenerator::KnightMovesFrom(*this, kingSpace))
	{
		Piece attacker = _state[move.end().ind()];

		if (attacker.type() == PieceType::Knight)
			return true;
	}

	return false;
}

// checking castling rights allows us to make the the following assumptions:
//  - the rook is still alive
//  - neither the rook nor king has moved
// so we need to check that
bool Board::IsMoveCastling(Move move)
{
	bool kingOnStart = PieceOnSpace(move.start()) == Piece(PieceType::King, _current_turn);

	int castleRank = _current_turn == PieceColor::White ? 0 : 7;
	bool validEndPoint = move.end() == Space(1, castleRank) || move.end() == Space(6, castleRank);

	return kingOnStart && validEndPoint;
}

// this function makes the assumption that the given move is legal
void Board::ApplyMove(Move move)
{
	Piece capturedPiece = _state[move.end().ind()];
	move.set_capture(capturedPiece);

	if (IsMoveCastling(move))
	{
		Space leftCastle = Space(0, _castle_rank());
		Space rightCastle = Space(6, _castle_rank());
		if (move.end() == leftCastle)
		{
			_state[move.end().ind()] = _state[move.start().ind()];
			_state[move.start().ind()] = Piece::None;
			_state[(leftCastle + Space(1, 0)).ind()] = _state[(leftCastle + Space(-1, 0)).ind()];
			_state[(leftCastle + Space(-1, 0)).ind()] = Piece::None;
		}
		if (move.end() == rightCastle)
		{
			_state[move.end().ind()] = _state[move.start().ind()];
			_state[move.start().ind()] = Piece::None;
			_state[(rightCastle + Space(-1, 0)).ind()] = _state[(rightCastle + Space(1, 0)).ind()];
			_state[(rightCastle + Space(1, 0)).ind()] = Piece::None;
		}
	}

	// castling rights mean that both the rook and king for that side have not moved
	// and have not been captured
	// remove castling rights if we move our king (castle or otherwise)
	if (PieceOnSpace(move.start()) == Piece(PieceType::King, _current_turn))
	{
		if (_current_turn == PieceColor::White)
		{
			if (_castling_rights[0])
				move.set_castling_change(0);
			if (_castling_rights[1])
				move.set_castling_change(1);
		}
		else
		{
			if (_castling_rights[2])
				move.set_castling_change(2);
			if (_castling_rights[3])
				move.set_castling_change(3);
		}
	}
	// remove castling rights on a side if we move our rook
	int castleInd = _current_turn == PieceColor::White ? 0 : 2;
	for (int i = castleInd; i < castleInd + 2; i++)
	{
		if (!_castling_rights[i] || move.start() != Board::castling_rook_spaces[i])
			continue;
		move.set_castling_change(i);
	}
	// remove opponents castling rights if a piece is taken
	// remember we don't need to check if its rook because it having castling r
	for (int i = (castleInd + 2) % 4; i < (castleInd + 2) % 4 + 2; i++)
	{
		if (!_castling_rights[i] || move.end() != Board::castling_rook_spaces[i])
			continue;
		move.set_castling_change(i);
	}

	_state[move.end().ind()] = _state[move.start().ind()];
	_state[move.start().ind()] = Piece::None;
	_passant_target = move.passant_target();

	if (move.promotion_flag() != PieceType::None)
	{
		_state[move.end().ind()] = Piece(move.promotion_flag(), _current_turn);
	}

	past_moves.push_back(move);
	_current_turn = OppositeColor(_current_turn);
}

void Board::UnapplyMove()
{
	Move lastMove = past_moves.back();

	// default case
	_state[lastMove.start().ind()] = _state[lastMove.end().ind()]; // move the piece back to start
	_state[lastMove.end().ind()] = lastMove.capture();			   // add the captured piece back (if any)
	if (past_moves.size() > 1)									   // revert back to previous passant target
		_passant_target = past_moves[past_moves.size() - 2].passant_target();
	else
		_passant_target = Space();

	// if it was promoted, we have to revert it back to a pawn
	if (lastMove.promotion_flag() != PieceType::None)
	{
		_state[lastMove.start().ind()] = Piece(PieceType::Pawn, _current_turn);
	}

	past_moves.pop_back();
	_current_turn = OppositeColor(_current_turn);
}

// TODO: check if current turn king will be in check, otherwise then we're good
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
#ifndef PIECE_H
#define PIECE_H

#include <cctype>
#include <cstdint>

enum class PieceType
{
	None,
	Pawn,
	Knight,
	Bishop,
	Rook,
	Queen,
	King
};

enum class PieceColor
{
	White = 0,
	Black = 8,
	None = 16
};

class Piece
{
private:
	PieceType _type = PieceType::None;
	PieceColor _color = PieceColor::None;

public:
	constexpr Piece() {}
	constexpr Piece(PieceType type_, PieceColor color_) : _type(type_), _color(color_) {}

	constexpr PieceType type() const
	{
		return _type;
	}

	constexpr PieceColor color() const
	{
		return _color;
	}

	constexpr bool operator==(const Piece &other) const
	{
		return _type == other.type() && _color == other.color();
	}

	constexpr bool operator!=(const Piece &other) const
	{
		return _type != other.type() && _color != other.color();
	}

	static constexpr bool charIsUppercase(char c)
	{
		return c >= 'A' && c <= 'Z';
	}

	static constexpr Piece pieceFromChar(char c)
	{
		PieceColor color = charIsUppercase(c) ? PieceColor::White : PieceColor::Black;
		PieceType type = PieceType::None;

		switch (std::tolower(static_cast<unsigned char>(c)))
		{
		case 'k':
			type = PieceType::King;
			break;
		case 'q':
			type = PieceType::Queen;
			break;
		case 'r':
			type = PieceType::Rook;
			break;
		case 'b':
			type = PieceType::Bishop;
			break;
		case 'n':
			type = PieceType::Knight;
			break;
		case 'p':
			type = PieceType::Pawn;
			break;
		}

		return Piece(type, color);
	}

	static constexpr char charFromPiece(Piece p)
	{
		char res = '.';

		switch (p.type())
		{
		case PieceType::King:
			res = 'k';
			break;
		case PieceType::Queen:
			res = 'q';
			break;
		case PieceType::Rook:
			res = 'r';
			break;
		case PieceType::Bishop:
			res = 'b';
			break;
		case PieceType::Knight:
			res = 'n';
			break;
		case PieceType::Pawn:
			res = 'p';
			break;
		default:
			return '.';
		}

		if (p.color() == PieceColor::White)
			res = static_cast<char>(std::toupper(static_cast<unsigned char>(res)));
		return res;
	}
};

#endif
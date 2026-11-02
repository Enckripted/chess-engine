#ifndef PIECE_H
#define PIECE_H

#include <cctype>
#include <cstdint>

enum PieceType
{
	None,
	Pawn,
	Knight,
	Bishop,
	Rook,
	Queen,
	King
};

enum PieceColor
{
	White = 0,
	Black = 8,
};

class Piece
{
private:
	std::uint8_t value;

public:
	constexpr Piece() : value(0) {}
	constexpr explicit Piece(std::uint8_t encoded) : value(encoded) {}
	constexpr Piece(PieceType type, PieceColor color) : value(static_cast<std::uint8_t>(color) | static_cast<std::uint8_t>(type)) {}

	constexpr std::uint8_t raw() const
	{
		return value;
	}

	constexpr PieceType type() const
	{
		return static_cast<PieceType>(value & 0b0111);
	}

	constexpr PieceColor color() const
	{
		return static_cast<PieceColor>(value & 0b1000);
	}

	constexpr explicit operator bool() const
	{
		return value != 0;
	}

	constexpr bool operator==(const Piece &other) const
	{
		return value == other.value;
	}

	constexpr bool operator!=(const Piece &other) const
	{
		return value != other.value;
	}

	static constexpr Piece encode(PieceType type, PieceColor color)
	{
		return Piece(static_cast<std::uint8_t>(color) | static_cast<std::uint8_t>(type));
	}

	static constexpr PieceType type(Piece piece)
	{
		return piece.type();
	}

	static constexpr PieceColor color(Piece piece)
	{
		return piece.color();
	}

	static constexpr bool charIsUppercase(char c)
	{
		return c >= 'A' && c <= 'Z';
	}

	static constexpr Piece pieceFromChar(char c)
	{
		PieceColor color = charIsUppercase(c) ? White : Black;
		PieceType type = None;

		switch (std::tolower(static_cast<unsigned char>(c)))
		{
		case 'k':
			type = King;
			break;
		case 'q':
			type = Queen;
			break;
		case 'r':
			type = Rook;
			break;
		case 'b':
			type = Bishop;
			break;
		case 'n':
			type = Knight;
			break;
		case 'p':
			type = Pawn;
			break;
		default:
			return Piece{};
		}

		return encode(type, color);
	}

	static constexpr char charFromPiece(Piece p)
	{
		char res = '.';

		switch (type(p))
		{
		case King:
			res = 'k';
			break;
		case Queen:
			res = 'q';
			break;
		case Rook:
			res = 'r';
			break;
		case Bishop:
			res = 'b';
			break;
		case Knight:
			res = 'n';
			break;
		case Pawn:
			res = 'p';
			break;
		default:
			return '.';
		}

		if (color(p) == White)
			res = static_cast<char>(std::toupper(static_cast<unsigned char>(res)));
		return res;
	}
};

#endif
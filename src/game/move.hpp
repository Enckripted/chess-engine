#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <array>

#include "piece.hpp"
#include "space.hpp"

class Move
{
private:
	Space _start = Space(0, 0);
	Space _end = Space(0, 0);
	Piece _capture = Piece::None;

public:
	inline Space start() const
	{
		return _start;
	};
	inline Space end() const
	{
		return _end;
	}
	inline Piece capture() const
	{
		return _capture;
	}

	inline void set_capture(Piece capture)
	{
		_capture = capture;
	}

	// https://en.wikipedia.org/wiki/Universal_Chess_Interface#Design
	// TODO: maybe a version that uses algebraic notation?
	Move(Space start, Space end) : _start(start), _end(end) {}
	Move(std::string uciMove);
	Move() = default;

	std::string ToString();

	inline bool operator==(const Move &other) const
	{
		return (*this).start() == other.start() && (*this).end() == other.end();
	}
};

#endif
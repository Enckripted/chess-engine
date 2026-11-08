#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <array>

#include "space.hpp"

class Move
{
private:
	Space _start = Space(0, 0);
	Space _end = Space(0, 0);

public:
	inline Space start() const
	{
		return _start;
	};
	inline Space end() const
	{
		return _end;
	}

	// https://en.wikipedia.org/wiki/Universal_Chess_Interface#Design
	// TODO: maybe a version that uses algebraic notation?
	Move(Space start, Space end) : _start(start), _end(end) {}
	Move(std::string uciMove);
	Move() = default;

	inline bool operator==(const Move &other) const
	{
		return (*this).start() == other.start() && (*this).end() == other.end();
	}
};

#endif
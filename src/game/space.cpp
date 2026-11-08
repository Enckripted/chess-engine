#include "space.hpp"

#include <array>
#include <string>
#include <iostream>

Space::Space(int ind)
{
	if (ind < 0 || ind >= 64)
		throw std::invalid_argument("invalid space index: " + std::to_string(ind));

	_file = ind % 8;
	_rank = ind / 8;
}

Space::Space(std::string algStr)
{
	_file = FileFromChar(algStr[0]);
	_rank = RankFromChar(algStr[1]);
}

std::string Space::ToString()
{
	return std::string{} + '(' + std::to_string(_file) + ',' + std::to_string(_rank) + ')';
}

std::string Space::ToStringNotation()
{
	if (!is_valid())
		return "XX";
	return std::string{kFileChars[_file], kRankChars[_rank]};
}
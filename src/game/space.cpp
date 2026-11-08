#include "space.hpp"

#include <array>
#include <string>

Space::Space(int ind)
{
	if (ind < 0 || ind >= 64)
		throw std::invalid_argument("invalid space index: " + std::to_string(ind));

	_rank = ind / 8;
	_file = ind % 8;
}

Space::Space(std::string algStr)
{
	_rank = RankFromChar(algStr[1]);
	_file = RankFromChar(algStr[0]);
}

std::string Space::ToString()
{
	return std::string{} + '(' + std::to_string(_rank) + ',' + std::to_string(_file) + ')';
}

std::string Space::ToStringNotation()
{
	if (!is_valid())
		return "XX";
	return std::string{kFileChars[_file], kRankChars[_rank]};
}
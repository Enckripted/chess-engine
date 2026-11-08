#include "space.hpp"

#include <array>
#include <string>

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
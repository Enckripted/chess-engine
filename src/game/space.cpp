#include "space.hpp"

#include <array>
#include <string>
#include <iostream>

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
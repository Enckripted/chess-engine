#ifndef SPACE_H
#define SPACE_H

#include <stdexcept>
#include <string>
#include <array>

class Space
{
private:
	static constexpr int RankFromChar(char rank_char)
	{
		int res = rank_char - '0';
		if (res < 0 || res >= 8)
			throw std::invalid_argument("invalid rank: " + rank_char);
		return res;
	}
	static constexpr int FileFromChar(char file_char)
	{
		int res = file_char - 'a';
		if (res < 0 || res >= 8)
			throw std::invalid_argument("invalid file: " + file_char);
		return res;
	}

	int _rank;
	int _file;

public:
	static constexpr std::array<char, 8> kRankChars{'1', '2', '3', '4', '5', '6', '7', '8'};
	static constexpr std::array<char, 8> kFileChars{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

	bool is_valid() const
	{
		return _rank >= 0 && _rank < 8 && _file >= 0 && _file < 8;
	}

	int rank() const
	{
		return _rank;
	}
	int file() const
	{
		return _file;
	}
	int ind() const
	{
		return _rank * 8 + _file;
	}

	Space(int ind);
	constexpr Space(int rank, int file) : _rank(rank), _file(file) {}
	Space(std::string algStr);

	std::string ToString();
	std::string ToStringNotation();

	inline Space &operator+=(const Space &other)
	{
		_rank += other.rank();
		_file += other.file();
		return *this;
	}
	inline bool operator==(const Space &other) const
	{
		return rank() == other.rank() && file() == other.file();
	}

	friend Space operator+(Space space, const Space &other)
	{
		space += other;
		return space;
	};
};

#endif
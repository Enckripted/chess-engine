#include "perft.hpp"

#include <cassert>
#include <iostream>

#include "game/board.hpp"
#include "move_generator.hpp"

long long perft(Board &board, int depth)
{
	if (depth == 0)
		return 1;

	auto moves = MoveGenerator::GenerateLegalMoves(board);
	long long ans = 0;
	for (auto move : moves)
	{
		if (board.MakeMove(move))
		{
			ans += perft(board, depth - 1);
			board.UnmakeMove();
		}
	}
	return ans;
}

bool RunPerftTest(std::string fen, int depth, std::vector<long long> expected)
{
	assert(depth <= expected.size());

	Board board(fen);
	bool success = true;

	std::cout << "Running perft at depth " << depth << " with FEN " << fen << std::endl;
	for (int i = 0; i < depth; i++)
	{
		long long res = perft(board, i + 1);
		success = res == expected[i] && success;
		std::cout << "Depth " << i + 1 << ": " << (res == expected[i] ? "OK" : "FAILED");
		std::cout << " - Nodes: " << res << "/Expected: " << expected[i] << std::endl;
	}
	std::cout << "OVERALL: " << (success ? "OK" : "FAILED") << "\n"
			  << std::endl;
	// std::cout << board.ToString() << std::endl;
	return success;
}

void RunPerftSuite()
{
	// RunPerftTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4,
	//			 {20, 400, 8902, 197281});
	RunPerftTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 3,
				 {48, 2039, 97862});
}
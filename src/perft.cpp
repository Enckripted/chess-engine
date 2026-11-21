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
		// std::cout << depth << " " << move.ToString() << std::endl;
		board.ApplyMove(move);
		ans += perft(board, depth - 1);
		board.UnmakeMove();
	}
	// std::cout << board.ToString() << std::endl;
	return ans;
}

void PerftDivide(std::string fen, int depth)
{
	std::cout << "Running perft divided at depth " << depth << " with FEN " << fen << std::endl;

	Board board(fen);
	std::vector<Move> moves = MoveGenerator::GenerateLegalMoves(board);
	for (auto move : moves)
	{
		board.ApplyMove(move);

		long long res = perft(board, depth);
		std::cout << move.ToString() << ": " << res << std::endl;

		board.UnapplyMove();
	}
	std::cout << board.ToString() << std::endl;
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

bool temp = false;
void RunPerftSuite()
{
	if (temp)
	{

		// PerftDivide("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 0);
		//  RunPerftTest("8/2p5/3p4/KP5r/1R2Pp1k/8/6P1/8 b - e4 1 1", 1, {16});
		return;
	}

	RunPerftTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4,
				 {20, 400, 8902, 197281});
	RunPerftTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 3,
				 {48, 2039, 97862});
	RunPerftTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4,
				 {14, 191, 2812, 43238});
	RunPerftTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 3,
				 {6, 264, 9467});
	//    infamous position 5
	RunPerftTest("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 3,
				 {44, 1486, 62379});
}
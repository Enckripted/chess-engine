#include "game_result.hpp"

#include <iostream>

#include "../move_generator.hpp"

GameResult GameResultEvaluator::EvaluateBoard(Board &board)
{
	auto moves = MoveGenerator::GenerateLegalMoves(board);

	// check for checkamte first
	if (moves.size() == 0 && board.CurrentlyInCheck(board.current_turn()))
		return board.current_turn() == PieceColor::Black ? GameResult::WinWhite : GameResult::WinBlack;

	// stalemate is just no legal moves but not in check
	if (moves.size() == 0)
		return GameResult::Draw;

	return GameResult::InProgress;
}
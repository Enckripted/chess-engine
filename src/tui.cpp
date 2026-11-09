#include "tui.hpp"

#include <iostream>

#include "game/game_result.hpp"

void TerminalUi::PrintGameState()
{
	std::cout << gameBoard.ToString() << std::endl;
	std::cout << std::endl;
	std::cout << (gameBoard.current_turn() == PieceColor::White ? "White to move" : "Black to move") << std::endl;
	if (gameBoard.CurrentlyInCheck(gameBoard.current_turn()))
		std::cout << "You are in check" << std::endl;
	std::cout << "\nEnter your move in UCI format (e.g. e2e4, 'exit' to stop):" << std::endl;
	std::cout << "Promotions are marked by adding an extra letter (b,n,r,q)" << std::endl;
}

void TerminalUi::StartGame()
{
	gameBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	PlayTurn();
}

void TerminalUi::PrintEndScreen()
{
	switch (GameResultEvaluator::EvaluateBoard(gameBoard))
	{
	case GameResult::WinWhite:
		std::cout << "WHITE WINS" << std::endl;
		break;
	case GameResult::WinBlack:
		std::cout << "WHITE WINS" << std::endl;
		break;
	default:
		std::cout << "Stalemate: you're all losers";
	}
	std::cout << "Enter to close window" << std::endl;
	std::cin;
}

void TerminalUi::PlayTurn()
{
	if (GameResultEvaluator::EvaluateBoard(gameBoard) != GameResult::InProgress)
	{
		PrintEndScreen();
		return;
	}

	PrintGameState();

	std::string userInput;
	std::cin >> userInput;
	if (userInput == "exit")
		return;

	try
	{
		Move playerMove = Move(userInput);
		bool madeMove = gameBoard.MakeMove(playerMove);
		if (!madeMove)
			std::cout << "Move invalid, try again" << std::endl;
	}
	catch (const char *msg)
	{
	}

	PlayTurn();
}
#include "ai/training/MatchRunner.h"
#include "ai/strategy/RandomStrategy.h"
#include "ai/strategy/MinimaxStrategy.h"
#include "ai/evaluate/HeuristicEvaluator.h"
#include <iostream>

int main()
{
    std::cerr << "START\n";
    HeuristicEvaluator eval;

    MinimaxStrategy bot1(&eval);
    RandomStrategy bot2;

    MatchRunner runner;

    GameResult result = runner.runMatch(bot1, bot2);

    std::cout << "Winner: ";

    if (result.winner == CellState::X)
        std::cout << "X";
    else if (result.winner == CellState::O)
        std::cout << "O";
    else
        std::cout << "Draw";

    std::cout << "\nMoves played: " << result.moveCount << std::endl;

    system("pause");
    return 0;
}

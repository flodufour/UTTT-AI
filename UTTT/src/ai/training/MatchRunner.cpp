#include "ai/training/MatchRunner.h"
#include <iostream>

GameResult MatchRunner::runMatch(IStrategy& strategyA, IStrategy& strategyB)
{
    GameState state;
    state.reset();

    state.setPlayers(CellState::X);

    int moveCount = 0;

    while (!state.isTerminal())
    {
        IStrategy* currentStrategy = nullptr;

        if (state.getCurrentPlayer() == CellState::X)
            currentStrategy = &strategyA;
        else
            currentStrategy = &strategyB;

        bool success = playTurn(state, *currentStrategy);

        if (!success)
        {
            std::cerr << "Invalid move played during simulation\n";

            CellState winner =
                (state.getCurrentPlayer() == CellState::X)
                ? CellState::O
                : CellState::X;

            return {winner, moveCount};
        }

        moveCount++;
    }

    return {
        state.getWinner(),
        moveCount
    };
}

bool MatchRunner::playTurn(GameState& state, IStrategy& strategy)
{
    AIMove move = strategy.chooseMove(state);

    if (!move.isValid())
        return false;

    return state.applyMove(move);
}

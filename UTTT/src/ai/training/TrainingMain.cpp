#include "ai/strategy/MinimaxStrategy.h"
#include "ai/evaluate/HeuristicEvaluator.h"
#include "ai/training/DataLogger.h"
#include "core/GameState.h"

#include <iostream>

int main()
{
    std::cerr << "=== GAME LOG START ===\n";

    DataLogger logger("game.json");

    GameState state;
    state.reset();

    logger.startGame();

    while (!state.isTerminal())
    {
        logger.addState(state);

        MinimaxStrategy* current =
            (state.getCurrentPlayer() == CellState::X)
            ? &playerX
            : &playerO;

        AIMove move = current->chooseMove(state);
        state.applyMove(move);
    }

    logger.addState(state);

    int result = 0;
    if (state.getWinner() == CellState::X) result = 1;
    else if (state.getWinner() == CellState::O) result = -1;

    logger.endGame(result);
    logger.save();

    std::cerr << "Game saved to game.json\n";
    std::cerr << "=== GAME LOG END ===\n";

    return 0;
}

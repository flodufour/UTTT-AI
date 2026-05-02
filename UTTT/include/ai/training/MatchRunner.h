#pragma once

#include "core/GameState.h"
#include "ai/strategy/IStrategy.h"

struct GameResult
{
    CellState winner;
    int moveCount;
};

class MatchRunner
{
public:

    /**
     * @brief Runs a full match between two strategies.
     *
     * Player X uses strategyA
     * Player O uses strategyB
     *
     * @param strategyA strategy controlling X
     * @param strategyB strategy controlling O
     * @return final game result
     */
    GameResult runMatch(IStrategy& strategyA, IStrategy& strategyB);

private:

    /**
     * @brief Plays one turn for the current player.
     *
     * @param state current game state
     * @param strategy strategy used for move selection
     * @return true if move was successfully applied
     */
    bool playTurn(GameState& state, IStrategy& strategy);
};

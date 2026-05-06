#include "ai/strategy/RandomStrategy.h"
#include <cstdlib>


AIMove RandomStrategy::chooseMove( GameState& state)
{
    auto moves = state.getValidMoves();

    if (moves.empty())
        return {-1, -1};

    int index = std::rand() % moves.size();
    return moves[index];
}

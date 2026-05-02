#include "ai/strategy/SimpleStrategy.h"


AIMove SimpleStrategy::chooseMove(const GameState& state)
{
    auto moves = state.getValidMoves();

    if (moves.empty())
        return {-1, -1};

    AIMove bestMove = moves[0];
    int bestScore = -1000000;

    for (const auto& move : moves)
    {
        GameState copy = state;
        copy.applyMove(move);

        int score = eval.evaluate(copy);

        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}

#include "ai/strategy/SimpleStrategy.h"

#include <limits>

SimpleStrategy::SimpleStrategy(IEvaluator* evaluator)
    : _evaluator(evaluator)
{
}

AIMove SimpleStrategy::chooseMove(GameState& state)
{
    auto moves = state.getValidMoves();

    if (moves.empty())
        return AIMove(-1, -1);

    AIMove bestMove = moves[0];
    int bestScore = std::numeric_limits<int>::min();

    for (const auto& move : moves)
    {
        GameState tmp = state;

        if (!tmp.applyMove(move))
            continue;

        int score = _evaluator->evaluate(tmp);

        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}

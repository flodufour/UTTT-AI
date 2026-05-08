#include "ai/strategy/MinimaxStrategy.h"

#include <limits>

MinimaxStrategy::MinimaxStrategy(IEvaluator* evaluator, int depth)
    : _evaluator(evaluator), _maxDepth(depth)
{
}

AIMove MinimaxStrategy::chooseMove(GameState& state)
{
    auto moves = state.getValidMoves();

    AIMove bestMove = moves[0];
    int bestScore = std::numeric_limits<int>::min();

    for (const auto& move : moves)
    {
        GameState tmp = state;

        if (!tmp.applyMove(move))
            continue;

        int score = minimax(tmp, _maxDepth - 1, false);

        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}

int MinimaxStrategy::minimax(GameState& state, int depth, bool maximizing)
{
    if (state.isTerminal() || depth == 0)
        return _evaluator->evaluate(state);

    auto moves = state.getValidMoves();

    if (moves.empty())
        return _evaluator->evaluate(state);

    if (maximizing)
    {
        int best = std::numeric_limits<int>::min();

        for (const auto& move : moves)
        {
            GameState tmp = state;
            if (!tmp.applyMove(move))
                continue;

            best = std::max(best, minimax(tmp, depth - 1, false));
        }

        return best;
    }
    else
    {
        int best = std::numeric_limits<int>::max();

        for (const auto& move : moves)
        {
            GameState tmp = state;
            if (!tmp.applyMove(move))
                continue;

            best = std::min(best, minimax(tmp, depth - 1, true));
        }

        return best;
    }
}

#include "ai/strategy/MinimaxStrategy.h"

#include <limits>
#include <iostream>
#include <algorithm>

MinimaxStrategy::MinimaxStrategy(IEvaluator* evaluator, int depth)
    : _evaluator(evaluator), _maxDepth(depth)
{
}

AIMove MinimaxStrategy::chooseMove(GameState& state)
{
    auto moves = state.getValidMoves();

    std::sort(moves.begin(), moves.end(),
    [&](const AIMove& a, const AIMove& b)
{
    int scoreA;
    int scoreB;

    auto moveA = state.applyMoveFast(a);
    scoreA = _evaluator->evaluate(state);
    state.undoMove(moveA);

    auto moveB = state.applyMoveFast(b);
    scoreB = _evaluator->evaluate(state);
    state.undoMove(moveB);

    return scoreA > scoreB;
    });

    AIMove bestMove = moves[0];
    int bestScore = std::numeric_limits<int>::min();

    int alpha = std::numeric_limits<int>::min();
    int beta  = std::numeric_limits<int>::max();

    for (const auto& move : moves)
    {
        GameState tmp = state;

        if (!tmp.applyMove(move))
            continue;

        int score = minimax(
            tmp,
            _maxDepth - 1,
            false,
            alpha,
            beta
        );

        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }

        alpha = std::max(alpha, bestScore);
    }

    std::cout << "best = " << bestScore << std::endl;
    return bestMove;
}

int MinimaxStrategy::minimax(
    GameState& state,
    int depth,
    bool maximizing,
    int alpha,
    int beta)
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

            best = std::max(
                best,
                minimax(
                    tmp,
                    depth - 1,
                    false,
                    alpha,
                    beta
                )
            );

            alpha = std::max(alpha, best);

            if (beta <= alpha)
                break; // prune
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

            best = std::min(
                best,
                minimax(
                    tmp,
                    depth - 1,
                    true,
                    alpha,
                    beta
                )
            );

            beta = std::min(beta, best);

            if (beta <= alpha)
                break; // prune
        }

        return best;
    }
}

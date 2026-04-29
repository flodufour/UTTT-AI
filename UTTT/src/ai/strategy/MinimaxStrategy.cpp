#include "ai/strategy/MinimaxStrategy.h"
#include <algorithm>

AIMove MinimaxStrategy::chooseMove(const GameState& state)
{

    auto moves = state.getValidMoves();

    if (state.getBoard().isEmpty()){
        return {4,4};
    }

    if (moves.empty())
        return {-1, -1};

    AIMove bestMove = moves[0];
    int bestScore = -1000000;

    for (const auto& move : moves)
    {
        GameState copy = state;
        copy.applyMove(move);

        int score = minimax(copy, 5, -1000000, 1000000, false);

        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}

int MinimaxStrategy::minimax(GameState state, int depth, int alpha, int beta, bool maximizing)
{
    if (depth == 0 || state.isTerminal())
        return _eval.evaluate(state);

    auto moves = state.getValidMoves();

    if (moves.empty())
        return _eval.evaluate(state);

    if (maximizing)
    {
        int best = -1000000;

        for (const auto& move : moves)
        {
            GameState copy = state;
            copy.applyMove(move);

            int score = minimax(copy, depth - 1, alpha, beta, false);

            best = std::max(best, score);
            alpha = std::max(alpha, score);

            if (beta <= alpha)
                break;
        }

        return best;
    }
    else
    {
        int best = 1000000;

        for (const auto& move : moves)
        {
            GameState copy = state;
            copy.applyMove(move);

            int score = minimax(copy, depth - 1, alpha, beta, true);

            best = std::min(best, score);
            beta = std::min(beta, score);

            if (beta <= alpha)
                break;
        }

        return best;
    }
}

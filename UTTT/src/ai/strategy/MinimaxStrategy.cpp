#include "ai/strategy/MinimaxStrategy.h"
#include <algorithm>

MinimaxStrategy::MinimaxStrategy(std::shared_ptr<IEvaluator> eval)
    : _eval(std::move(eval))
{
}

AIMove MinimaxStrategy::chooseMove(GameState& state)
{
    _start = std::chrono::steady_clock::now();

    auto moves = state.getValidMoves();
    if (moves.empty())
        return {-1, -1};

    AIMove bestMove = moves[0];
    int bestScore = -INF;

    for (int depth = 1; depth <= MAX_DEPTH; depth++)
    {
        if (isTimeUp())
            break;

        for (const auto& move : moves)
        {
            auto undo = state.applyMoveFast(move);

            int score = minimax(state, depth - 1, -INF, INF, false);

            state.undoMove(undo);

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = move;
            }
        }
    }

    return bestMove;
}


int MinimaxStrategy::minimax(GameState& state,
                             int depth,
                             int alpha,
                             int beta,
                             bool maximizing)
{
    uint64_t hash = state.getHash();

    auto it = _tt.find(hash);
    if (it != _tt.end())
    {
        const TTEntry& e = it->second;

        if (e.depth >= depth)
            return e.value;
    }

    if (state.isTerminal())
        return _eval->evaluate(state);

    if (depth == 0 || isTimeUp())
        return _eval->evaluate(state);

    // =========================
    // SEARCH
    // =========================
    int best = maximizing ? -INF : INF;

    auto moves = state.getValidMoves();

    for (const auto& move : moves)
    {
        auto undo = state.applyMoveFast(move);

        int score = minimax(state, depth - 1, alpha, beta, !maximizing);

        state.undoMove(undo);

        if (maximizing)
        {
            best = std::max(best, score);
            alpha = std::max(alpha, score);
        }
        else
        {
            best = std::min(best, score);
            beta = std::min(beta, score);
        }

        if (beta <= alpha)
            break;
    }

    _tt[hash] = { best, depth, true };

    return best;
}

bool MinimaxStrategy::isTimeUp() const
{
    return elapsedMs() >= _maxTimeMs;
}

int MinimaxStrategy::elapsedMs() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - _start
    ).count();
}

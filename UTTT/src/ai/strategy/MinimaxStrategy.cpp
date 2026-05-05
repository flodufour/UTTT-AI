#include "ai/strategy/MinimaxStrategy.h"
#include <algorithm>
#include <iostream>


MinimaxStrategy::MinimaxStrategy(std::shared_ptr<IEvaluator> eval)
    : _eval(std::move(eval))
{
    if (!_eval)
        return;
}


AIMove MinimaxStrategy::chooseMove(const GameState& state)
{
    _start = std::chrono::steady_clock::now();
    _rootPlayer = state.getMyPlayer();

    auto moves = state.getValidMoves();
    if (moves.empty())
        return {-1, -1};

    AIMove bestMove = moves[0];
    int bestScore = -INF;

    lastCompletedMove = moves[0];
    lastCompletedScore = -INF;

    for (int depth = 1; depth <= MAX_DEPTH; depth++)
    {
        if (isTimeUp())
            break;

        std::sort(moves.begin(), moves.end(),
            [&](const AIMove& a, const AIMove& b)
            {
                return moveOrderingScore(state, a) > moveOrderingScore(state, b);
            });

        AIMove localBest = moves[0];
        int localBestScore = -INF;

        bool completedDepth = true;

        for (const auto& move : moves)
        {
            if (isTimeUp())
            {
                completedDepth = false;
                break;
            }

            GameState next = state;
            next.applyMove(move);

            int score = minimax(next, depth - 1, -INF, INF, false, 0);

            if (score > localBestScore)
            {
                localBestScore = score;
                localBest = move;
            }
        }

        if (completedDepth && !isTimeUp())
        {
            bestMove = localBest;
            bestScore = localBestScore;

            lastCompletedMove = localBest;
            lastCompletedScore = localBestScore;
        }
        else
        {
            break;
        }
    }

    return lastCompletedMove;
}

int MinimaxStrategy::minimax(GameState state,
                            int depth,
                            int alpha,
                            int beta,
                            bool maximizing,
                            int ply)
{
    if (state.isTerminal())
    {
        int win = _eval->evaluate(state);

        if (win < 0)
            return win + ply;

        return win - ply;
    }

    if (depth == 0)
        return _eval->evaluate(state);

    if (isTimeUp())
        return _eval->evaluate(state);

    auto moves = state.getValidMoves();
    if (moves.empty())
        return _eval->evaluate(state);

    std::sort(moves.begin(), moves.end(),
        [&](const AIMove& a, const AIMove& b)
        {
            return moveOrderingScore(state, a) > moveOrderingScore(state, b);
        });

    if (maximizing)
    {
        int best = -INF;

        for (const auto& move : moves)
        {
            if (isTimeUp()) break;

            GameState next = state;
            next.applyMove(move);

            int score = minimax(next, depth - 1, alpha, beta, false, ply + 1);

            best = std::max(best, score);
            alpha = std::max(alpha, score);

            if (beta <= alpha)
                break;
        }

        return best;
    }
    else
    {
        int best = INF;

        for (const auto& move : moves)
        {
            if (isTimeUp()) break;

            GameState next = state;
            next.applyMove(move);

            int score = minimax(next, depth - 1, alpha, beta, true, ply + 1);

            best = std::min(best, score);
            beta = std::min(beta, score);

            if (beta <= alpha)
                break;
        }

        return best;
    }
}


int MinimaxStrategy::moveOrderingScore(const GameState& state, const AIMove& move)
{
    int score = 0;

    if (move.cellIndex == 4)
        score += 100;

    if (move.boardIndex == 4)
        score += 80;

    if (move.boardIndex % 2 == 0)
        score += 20;

    if (move.cellIndex % 2 == 0)
        score += 10;

    return score;
}

bool MinimaxStrategy::isTimeUp() const
{
    return timeElapsedMs() >= _maxTimeMs;
}

int MinimaxStrategy::timeElapsedMs() const
{
    auto now = std::chrono::steady_clock::now();
    return (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - _start).count();
}

int MinimaxStrategy::evaluateStateForMCTS(const GameState& state, int depth)
{
    GameState copy = state;

    return minimax(
        copy,
        depth,
        -INF,
        INF,
        true,
        0
    );
}

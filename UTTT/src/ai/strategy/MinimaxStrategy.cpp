#include "ai/strategy/MinimaxStrategy.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

AIMove MinimaxStrategy::chooseMove(const GameState& state)
{
    _start = std::chrono::steady_clock::now();

    auto topMoves = getTopMoves(state);

    if (topMoves.empty())
        return {-1, -1};

    int randomIndex = rand() % topMoves.size();

    return topMoves[randomIndex].move;
}

std::vector<MinimaxStrategy::ScoredMove>
MinimaxStrategy::getTopMoves(const GameState& state)
{
    auto moves = state.getValidMoves();

    std::vector<ScoredMove> scoredMoves;

    if (moves.empty())
        return scoredMoves;

    AIMove bestMove = moves[0];
    int bestScoreGlobal = -1000000;

    for (int depth = 1; depth <= 5; depth++)
    {
        if (isTimeUp())
            break;

        scoredMoves.clear();

        std::sort(moves.begin(), moves.end(),
        [&](const AIMove& a, const AIMove& b)
        {
            return heuristicScore(state, a) > heuristicScore(state, b);
        });

        for (const auto& move : moves)
        {
            if (isTimeUp())
                break;

            GameState copy = state;
            copy.applyMove(move);

            int score = minimax(copy, depth - 1, -1000000, 1000000, false);

            scoredMoves.push_back({move, score});

            if (score > bestScoreGlobal)
            {
                bestScoreGlobal = score;
                bestMove = move;
            }
        }

        if (isTimeUp())
            break;
    }

    std::sort(scoredMoves.begin(), scoredMoves.end(),
        [](const ScoredMove& a, const ScoredMove& b)
        {
            return a.score > b.score;
        });

    if (scoredMoves.size() > _topK)
    {
        scoredMoves.resize(_topK);
    }

    return scoredMoves;
}

int MinimaxStrategy::minimax(GameState state, int depth, int alpha, int beta, bool maximizing)
{
    if (depth == 0 || state.isTerminal())
        return _eval->evaluate(state);

    if (isTimeUp())
        return _eval->evaluate(state);

    auto moves = state.getValidMoves();

    if (moves.empty())
        return _eval->evaluate(state);

    int best = maximizing ? -1000000 : 1000000;

    for (const auto& move : moves)
    {
        if (isTimeUp())
            break;

        GameState copy = state;
        copy.applyMove(move);

        int score = minimax(copy, depth - 1, alpha, beta, !maximizing);

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

    return best;
}

bool MinimaxStrategy::isTimeUp()
{
    auto now = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(
        now - _start
    ).count() >= _maxTimeMs;
}

int MinimaxStrategy::heuristicScore(const GameState& state, const AIMove& move)
{
    int score = 0;

    if (move.boardIndex == 4 && move.cellIndex == 4)
        score += 50;

    if (move.boardIndex % 3 == 1 && move.cellIndex % 3 == 1)
        score += 20;

    return score;
}

MinimaxStrategy::MinimaxStrategy(IEvaluator* eval)
{
    if (!eval)
        throw std::runtime_error("Evaluator is null");

    _eval = eval;

    srand(time(nullptr));
}

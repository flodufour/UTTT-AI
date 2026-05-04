#pragma once

#include "IStrategy.h"
#include "ai/evaluate/IEvaluator.h"
#include <chrono>
#include <memory>

class MinimaxStrategy : public IStrategy
{
public:
    explicit MinimaxStrategy(std::shared_ptr<IEvaluator> eval);

    AIMove chooseMove(const GameState& state) override;

private:
    int minimax(GameState state,
                int depth,
                int alpha,
                int beta,
                bool maximizing,
                int ply);

    int moveOrderingScore(const GameState& state, const AIMove& move);

    bool isTimeUp() const;
    int timeElapsedMs() const;

private:
    std::shared_ptr<IEvaluator> _eval;

    std::chrono::steady_clock::time_point _start;

    int _maxTimeMs = 10000;

    static constexpr int INF = 1'000'000;
    static constexpr int MAX_DEPTH = 30;
};

#pragma once

#include <memory>
#include <unordered_map>
#include <chrono>

#include "ai/strategy/IStrategy.h"
#include "ai/evaluate/IEvaluator.h"

class MinimaxStrategy : public IStrategy
{
public:
    explicit MinimaxStrategy(std::shared_ptr<IEvaluator> eval);

    AIMove chooseMove(GameState& state) override;

private:
    int minimax(GameState& state, int depth, int alpha, int beta, bool maximizing);

    bool isTimeUp() const;
    int elapsedMs() const;

private:
    struct TTEntry
    {
        int value;
        int depth;
        bool flagExact;
    };

    std::unordered_map<uint64_t, TTEntry> _tt;

    std::shared_ptr<IEvaluator> _eval;

    std::chrono::steady_clock::time_point _start;
    int _maxTimeMs = 1000;

    static constexpr int INF = 1'000'000;
    static constexpr int MAX_DEPTH = 10;
};

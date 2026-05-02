#pragma once
#include "IStrategy.h"
#include "ai/evaluate/IEvaluator.h"
#include <chrono>

class MinimaxStrategy : public IStrategy {
public:
    AIMove chooseMove(const GameState& state) override;

private:
    int minimax(GameState state, int depth, int alpha, int beta, bool maximizing);

    IEvaluator* _eval;

    std::chrono::steady_clock::time_point _start;
    int _maxTimeMs = 5000;

    bool isTimeUp();

    int heuristicScore(const GameState& state, const AIMove& move);
};

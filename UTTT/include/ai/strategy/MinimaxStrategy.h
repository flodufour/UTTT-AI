#pragma once
#include "IStrategy.h"
#include "ai/evaluate/IEvaluator.h"
#include <chrono>

class MinimaxStrategy : public IStrategy {
public:
    MinimaxStrategy(IEvaluator* eval);

    AIMove chooseMove(const GameState& state) override;

private:

    int minimax(GameState state, int depth, int alpha, int beta, bool maximizing);

    IEvaluator* _eval;

    std::chrono::steady_clock::time_point _start;
    int _maxTimeMs = 350;

    bool isTimeUp();

    int heuristicScore(const GameState& state, const AIMove& move);
};

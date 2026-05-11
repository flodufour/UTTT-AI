#pragma once

#include "core/AIMove.h"
#include "core/GameState.h"
#include "ai/strategy/IStrategy.h"
#include "ai/evaluate/IEvaluator.h"

class MinimaxStrategy : public IStrategy {
public:
    MinimaxStrategy(IEvaluator* evaluator, int depth = 3);

    AIMove chooseMove(GameState& state) override;

private:
    int minimax(
    GameState& state,
    int depth,
    bool maximizing,
    int alpha,
    int beta
);

    IEvaluator* _evaluator;
    int _maxDepth;
};

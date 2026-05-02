#pragma once
#include "IStrategy.h"
#include "ai/evaluate/HeuristicEvaluator.h"

class RandomStrategy : public IStrategy {
public:
    AIMove chooseMove(const GameState& state) override;

private:
    HeuristicEvaluator _eval;
};

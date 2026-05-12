#pragma once
#include "IStrategy.h"
#include "ai/evaluate/HeuristicEvaluator.h"

class RandomStrategy : public IStrategy {
public:
    AIMove chooseMove( GameState& state) override;

    void reset() override {
}

private:
    HeuristicEvaluator _eval;
};

#pragma once

#include "core/AIMove.h"
#include "core/GameState.h"
#include "ai/strategy/IStrategy.h"
#include "ai/evaluate/HeuristicEvaluator.h"

#include <memory>

class SimpleStrategy : public IStrategy {
public:
    explicit SimpleStrategy(IEvaluator* evaluator);

    AIMove chooseMove(GameState& state) override;

    void reset() override {
}

private:
    IEvaluator* _evaluator;
};

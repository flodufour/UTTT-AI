#pragma once
#include "IStrategy.h"
#include "../evaluate/Evaluation.h"

class RandomStrategy : public IStrategy {
public:
    AIMove chooseMove(const GameState& state) override;

private:
    Evaluation _eval;
};

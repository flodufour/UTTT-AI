#pragma once

#include "IStrategy.h"
#include "../evaluate/Evaluation.h"

class SimpleStrategy : public IStrategy {
public:
    AIMove chooseMove(const GameState& state) override;

private:
    Evaluation eval;
};

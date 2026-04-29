#pragma once
#include "IStrategy.h"
#include "ai/evaluate/Evaluation.h"

class MinimaxStrategy : public IStrategy {
public:
    AIMove chooseMove(const GameState& state) override;

private:
    int minimax(GameState state, int depth, int alpha, int beta, bool maximizing);
    Evaluation _eval;
};

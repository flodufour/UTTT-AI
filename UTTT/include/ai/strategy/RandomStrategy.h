#pragma once
#include "IStrategy.h"
#include "../evaluate/Evaluation.h"

class RandomStrategy : public IStrategy {
public:
    AIMove chooseMove(const GameState& state) override;

private:
    bool _isWinningMove(const GameState& state, const AIMove& move);
    bool _isBlockingMove(const GameState& state, const AIMove& move);

    Evaluation _eval;
};

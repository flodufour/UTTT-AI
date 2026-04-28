#pragma once

#include "core/GameState.h"

class Evaluation {
public:
    int evaluate(const GameState& state);

private:
    int evaluateSubBoard(const SubBoard& sb, CellState me);
};

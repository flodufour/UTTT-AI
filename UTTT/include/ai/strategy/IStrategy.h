#pragma once

#include "core/AIMove.h"
#include "core/GameState.h"

class IStrategy {
public:
    virtual AIMove chooseMove(const GameState& state) = 0;
    virtual ~IStrategy() = default;
};

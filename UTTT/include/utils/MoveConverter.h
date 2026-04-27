#pragma once

#include "main.h"
#include "core/AIMove.h"

class MoveConverter {
public:
    GameMove toGameMove(AIMove m);
    AIMove toAIMove(GameMove p);
};

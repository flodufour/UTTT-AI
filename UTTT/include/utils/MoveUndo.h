#include "core/AIMove.h"
#include <cstdint>


struct MoveUndo
{
    AIMove move;
    int prevActiveBoard;
};

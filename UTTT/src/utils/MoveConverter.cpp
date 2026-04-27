#include "utils/MoveConverter.h"

AIMove MoveConverter::toAIMove(GameMove p)
{
    int row = p.row;
    int col = p.col;

    int boardIndex = (row / 3) * 3 + (col / 3);
    int cellIndex  = (row % 3) * 3 + (col % 3);

    return AIMove(boardIndex, cellIndex);
}

GameMove MoveConverter::toGameMove(AIMove m)
{
    int br = m.boardIndex / 3;
    int bc = m.boardIndex % 3;

    int cr = m.cellIndex / 3;
    int cc = m.cellIndex % 3;

    return GameMove{br * 3 + cr, bc * 3 + cc };
}

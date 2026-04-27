#pragma once

struct AIMove
{
    int boardIndex;
    int cellIndex;

    AIMove() : boardIndex(-1), cellIndex(-1) {}

    AIMove(int b, int c) : boardIndex(b), cellIndex(c) {}

    bool isValid() const
    {
        return boardIndex >= 0 && boardIndex < 9 &&
               cellIndex >= 0 && cellIndex < 9;
    }
};

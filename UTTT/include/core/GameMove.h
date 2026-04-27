#pragma once

struct GameMove
{
    int boardIndex;
    int cellIndex;

    GameMove() : boardIndex(-1), cellIndex(-1) {}

    GameMove(int b, int c) : boardIndex(b), cellIndex(c) {}

    bool isValid() const
    {
        return boardIndex >= 0 && boardIndex < 9 &&
               cellIndex >= 0 && cellIndex < 9;
    }
};

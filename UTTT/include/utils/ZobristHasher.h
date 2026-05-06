#pragma once
#include <cstdint>

class Zobrist
{
public:
    static void init();

    static uint64_t cell[9][9][2];

    static uint64_t activeBoard[10];

    static uint64_t player[2];

    static int activeBoardIndex(int b);
};

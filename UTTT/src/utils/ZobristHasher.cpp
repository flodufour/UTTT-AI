#include "utils/ZobristHasher.h"
#include <random>

uint64_t Zobrist::cell[9][9][2];
uint64_t Zobrist::activeBoard[10];
uint64_t Zobrist::player[2];

static uint64_t rand64()
{
    static std::mt19937_64 rng(0xC0FFEE123456789ULL);
    static std::uniform_int_distribution<uint64_t> dist;
    return dist(rng);
}

void Zobrist::init()
{
    for (int b = 0; b < 9; ++b)
    {
        for (int c = 0; c < 9; ++c)
        {
            cell[b][c][0] = rand64(); // X
            cell[b][c][1] = rand64(); // O
        }
    }

    for (int i = 0; i < 10; ++i)
    {
        activeBoard[i] = rand64();
    }

    player[0] = rand64(); // X
    player[1] = rand64(); // O
}

int Zobrist::activeBoardIndex(int b)
{
    return (b == -1) ? 9 : b;
}

#include "utils/ZobristHasher.h"

// ============================================================
// STATIC STORAGE
// ============================================================

std::array<std::array<std::array<uint64_t, 2>, 9>, 9> ZobristHasher::pieceTable;
std::array<uint64_t, 9> ZobristHasher::activeBoardKey;
uint64_t ZobristHasher::turnKey = 0;
uint64_t ZobristHasher::anyBoardKey = 0;
bool ZobristHasher::initialized = false;

// ============================================================
// INIT
// ============================================================

void ZobristHasher::init()
{
    if (initialized)
        return;

    std::mt19937_64 rng(0xC0FFEE123456789ULL);

    // --------------------------------------------------------
    // PIECES [board][cell][player]
    // --------------------------------------------------------
    for (int b = 0; b < 9; b++)
    {
        for (int c = 0; c < 9; c++)
        {
            for (int p = 0; p < 2; p++)
            {
                pieceTable[b][c][p] = rng();
            }
        }
    }

    // --------------------------------------------------------
    // ACTIVE BOARD KEYS
    // --------------------------------------------------------
    for (int i = 0; i < 9; i++)
    {
        activeBoardKey[i] = rng();
    }

    // --------------------------------------------------------
    // TURN KEY
    // --------------------------------------------------------
    turnKey = rng();
    anyBoardKey = rng();

    initialized = true;
}

// ============================================================
// EMPTY HASH
// ============================================================

uint64_t ZobristHasher::hashEmpty()
{
    return 0ULL;
}

// ============================================================
// PIECE TOGGLE
// ============================================================

void ZobristHasher::togglePiece(uint64_t& hash,
                                 int board,
                                 int cell,
                                 int player)
{
    if (board < 0 || board >= 9) return;
    if (cell < 0 || cell >= 9) return;
    if (player < 0 || player > 1) return;

    hash ^= pieceTable[board][cell][player];
}

// ============================================================
// TURN TOGGLE
// ============================================================

void ZobristHasher::toggleTurn(uint64_t& hash)
{
    hash ^= turnKey;
}

// ============================================================
// ACTIVE BOARD TOGGLE
// ============================================================

void ZobristHasher::toggleActiveBoard(uint64_t& hash, int board)
{
    if (board == -1)
    {
        hash ^= anyBoardKey;
        return;
    }

    if (board >= 0 && board < 9)
    {
        hash ^= activeBoardKey[board];
    }
}

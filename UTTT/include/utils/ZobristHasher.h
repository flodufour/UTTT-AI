#pragma once

#include <cstdint>
#include <array>
#include <random>

class ZobristHasher
{
public:
    // ============================================================
    // INIT
    // ============================================================
    static void init();

    // ============================================================
    // HASH HELPERS
    // ============================================================
    static uint64_t hashEmpty();

    static void togglePiece(uint64_t& hash,
                            int board,
                            int cell,
                            int player);

    static void toggleTurn(uint64_t& hash);

    static void toggleActiveBoard(uint64_t& hash, int board);

    // ============================================================
    // PIECES
    // [board][cell][player]
    // player: 0 = X, 1 = O
    // ============================================================
    static std::array<std::array<std::array<uint64_t, 2>, 9>, 9> pieceTable;

    // ============================================================
    // GAME STATE KEYS
    // ============================================================
    static uint64_t turnKey;
    static std::array<uint64_t, 9> activeBoardKey;
    static uint64_t anyBoardKey;

    static bool initialized;
};

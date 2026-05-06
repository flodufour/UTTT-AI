#include "core/GameState.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <unordered_set>
#include "utils/ZobristHasher.h"

static void debugMove(GameState& s)
{

    Zobrist::init();

    auto moves = s.getValidMoves();
    std::cout << "mv s"<<moves.size()<< std::endl;
    if (moves.empty()) return;

    AIMove m = moves[rand() % moves.size()];

    uint64_t h1 = s.getHash();

    auto undo = s.applyMove(m);

    uint64_t h2 = s.getHash();

    std::cout << "MOVE: (" << m.boardIndex << "," << m.cellIndex << ")\n";
    std::cout << "HASH BEFORE: " << h1 << "\n";
    std::cout << "HASH AFTER : " << h2 << "\n";

    return;
    if (h1 == h2)
        std::cout << "❌ NO HASH CHANGE\n";

    //s.undoMove(undo);

    uint64_t h3 = s.getHash();

    if (h1 != h3)
        std::cout << "❌ UNDO BROKEN\n";
}

int main()
{
    srand(0);

    GameState s;
    s.setPlayers(CellState::X);

    std::cout << "=== DEBUG START ===\n";

    for (int i = 0; i < 100; i++)
        debugMove(s);


    std::cout << "=== DONE ===\n";

    system("pause");
}

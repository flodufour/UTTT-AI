#pragma once

#include "core/GameState.h"

class Evaluation {
public:
    int evaluate(const GameState& state);

private:
    int checkEndgame(const UltimateBoard&, CellState me, CellState opp);
    int evaluateAllSubBoards(const UltimateBoard& sb, CellState me, CellState opp);
    int evaluateSubBoard(const SubBoard& sb, CellState me);
    int evaluateMetaBoard(const UltimateBoard& b, CellState me);

    int boardWeight[9] = {
    3,2,3,
    2,4,2,
    3,2,3};
};

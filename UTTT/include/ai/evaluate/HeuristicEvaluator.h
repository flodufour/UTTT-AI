#pragma once

#include "core/GameState.h"
#include "IEvaluator.h"

/// @class HeuristicEvaluator
/// @brief Rule-based evaluation function for Ultimate Tic Tac Toe.
///
/// Computes a score using handcrafted heuristics such as:
/// - meta board control
/// - sub-board strength
/// - forced move pressure
class HeuristicEvaluator : public IEvaluator {
private:
    int evaluateTerminal(const UltimateBoard& b, CellState me, CellState opp);
    int evaluateMeta(const UltimateBoard& b, CellState me, CellState opp);
    int evaluateBoards(const UltimateBoard& b, CellState me, CellState opp);
    int evaluateSubBoard(const SubBoard& sb, CellState me);
    int evaluateForcedMove(const UltimateBoard& b, CellState me, CellState opp);

    int checkEndgame(const UltimateBoard& b, CellState me, CellState opp);

    public:


    struct Weights
    {
        int WIN = 100000;

        int META_THREE = 3000;
        int META_TWO   = 600;
        int META_ONE   = 100;

        int SUB_WIN = 150;
        int SUB_TWO = 30;

        int CENTER = 50;

        int FORCED_GOOD = 200;
        int FORCED_BAD = 250;

        int boardWeight[9] = {
        3,2,3,
        2,4,2,
        3,2,3
    };
    } _w;

    HeuristicEvaluator() = default;
    HeuristicEvaluator(const Weights& w);

    int evaluate(const GameState& state);

};

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

    int META_THREE = 8000;
    int META_TWO   = 1500;
    int META_ONE   = 300;


    int SUB_WIN = 200;
    int SUB_TWO = 250;

    int CENTER = 60;

    int CORNER = 20;

    int FORCED_GOOD = 300;
    int FORCED_BAD = 500;

    int boardWeight[9] = {
        3, 2, 3,
        2, 5, 2,
        3, 2, 3
    };
    } _w;

    HeuristicEvaluator() = default;
    HeuristicEvaluator(const Weights& w);

    int evaluate(const GameState& state);

};

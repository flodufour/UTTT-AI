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
public:
    int evaluate(const GameState& state);

private:
    int evaluateTerminal(const UltimateBoard& b, CellState me, CellState opp);
    int evaluateMeta(const UltimateBoard& b, CellState me, CellState opp);
    int evaluateBoards(const UltimateBoard& b, CellState me, CellState opp);
    int evaluateSubBoard(const SubBoard& sb, CellState me);
    int evaluateForcedMove(const UltimateBoard& b, CellState me, CellState opp);

    int checkEndgame(const UltimateBoard& b, CellState me, CellState opp);

private:
    int boardWeight[9] = {
        3,2,3,
        2,4,2,
        3,2,3
    };

    struct EvalWeights
    {
        static constexpr int WIN = 100000;

        static constexpr int META_THREE = 3000;
        static constexpr int META_TWO   = 600;
        static constexpr int META_ONE   = 100;

        static constexpr int SUB_WIN = 150;
        static constexpr int SUB_TWO = 30;

        static constexpr int CENTER = 50;

        static constexpr int FORCED_GOOD = 200;
        static constexpr int FORCED_BAD = 250;
    };
};

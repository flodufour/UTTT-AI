#pragma once

#include "core/GameState.h"
#include "IEvaluator.h"

/// @class HeuristicEvaluator
/// @brief Hierarchical evaluation for Ultimate Tic Tac Toe (MCTS-safe)
///
/// Design:
/// - SubBoard  : tactical evaluation (local patterns)
/// - MetaBoard : strategic evaluation (win conditions on boards)
/// - Forced    : tempo / constraint pressure
class HeuristicEvaluator : public IEvaluator
{
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

    struct PositionalWeights
    {
        static constexpr int WIN = 100000;

        static constexpr int META_THREE = 8000;
        static constexpr int META_TWO   = 1200;
        static constexpr int META_ONE   = 200;

        static constexpr int SUB_WIN = 500;
        static constexpr int SUB_TWO = 40;

        static constexpr int CENTER = 25;
        static constexpr int CORNER = 10;

        static constexpr int FORCED_GOOD = 120;
        static constexpr int FORCED_BAD  = 180;
    };


    int boardWeight[9] = {
        3, 4, 3,
        4, 6, 4,
        3, 4, 3
    };
};

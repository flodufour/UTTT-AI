#pragma once

#include "core/GameState.h"
#include "IEvaluator.h"
#include "core/UltimateBoard.h"
#include "core/SubBoard.h"

/// @class HeuristicEvaluator
/// @brief Stable hierarchical evaluator for Ultimate Tic Tac Toe
///
/// Design goals:
/// - stable scores (important for alpha-beta + TT)
/// - proportional weighting
/// - no extreme value explosions except true terminal wins
/// - MCTS + minimax safe

class HeuristicEvaluator : public IEvaluator
{
public:
    int evaluate(const GameState& state) const override;

private:
    int evaluateTerminal(const UltimateBoard& b, CellState me, CellState opp) const;
    int evaluateMeta(const UltimateBoard& b, CellState me, CellState opp) const;
    int evaluateBoards(const UltimateBoard& b, CellState me, CellState opp) const;
    int evaluateSubBoard(const SubBoard& sb, CellState me, CellState opp) const;
    int evaluateForcedMove(const UltimateBoard& b, CellState me, CellState opp) const;


private:
    struct W
    {
        // Global win
        static constexpr int WIN = 10000;

        // Meta board
        static constexpr int META_THREE = 1200;
        static constexpr int META_TWO   = 250;
        static constexpr int META_ONE   = 125;

        // Sub boards
        static constexpr int SUB_TWO = 25;
        static constexpr int SUB_ONE = 5;

        // Positional
        static constexpr int CENTER = 6;
        static constexpr int CORNER = 3;

        // Tempo / forcing
        static constexpr int FORCED_GOOD = 10;
        static constexpr int FORCED_VERY_GOOD = 15;
        static constexpr int FORCED_BAD = -15;
        static constexpr int FORCED_VERY_BAD = -25;

        static constexpr int FREE_MOVE = -40;
    };

    int boardWeight[9] = {
        3, 4, 3,
        4, 5, 4,
        3, 4, 3
    };

};

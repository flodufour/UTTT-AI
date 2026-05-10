#pragma once

#include "core/GameState.h"
#include "ai/evaluate/IEvaluator.h"

class FeatureEvaluator : public IEvaluator
{
public:
    int evaluate(const GameState& state) const override;

private:
    struct Features
    {
        // Terminal
        int terminalWin = 0;
        int terminalLoss = 0;

        // Meta board control
        int metaOwned = 0;
        int metaOpponentOwned = 0;

        // Meta threats
        int metaTwoInRow = 0;
        int metaOneInRow = 0;
        int metaOpponentTwoInRow = 0;
        int metaOpponentOneInRow = 0;

        // Meta forks
        int metaFork = 0;
        int metaOpponentFork = 0;

        // Sub-board positional control
        int subCellControl = 0;
        int subCellOpponentControl = 0;

        // Sub-board threats
        int subTwoInRow = 0;
        int subOneInRow = 0;
        int subOpponentTwoInRow = 0;
        int subOpponentOneInRow = 0;

        // Sub forks
        int subFork = 0;
        int subOpponentFork = 0;

        // Forced move system
        int forcedGood = 0;
        int forcedVeryGood = 0;
        int forcedBad = 0;
        int forcedVeryBad = 0;

        // Free move
        int freeMove = 0;

        // Meta importance pressure
        int metaImportanceGood = 0;
        int metaImportanceBad = 0;

        // Positional board control
        int boardPositionBonus = 0;
    };

    struct Weights
{
    int terminalWin;
    int terminalLoss;

    int metaOwned;
    int metaOpponentOwned;

    int metaTwoInRow;
    int metaOneInRow;
    int metaOpponentTwoInRow;
    int metaOpponentOneInRow;

    int metaFork;
    int metaOpponentFork;

    int subCellControl;
    int subCellOpponentControl;

    int subTwoInRow;
    int subOneInRow;
    int subOpponentTwoInRow;
    int subOpponentOneInRow;

    int subFork;
    int subOpponentFork;

    int forcedGood;
    int forcedVeryGood;
    int forcedBad;
    int forcedVeryBad;

    int freeMove;

    int metaImportanceGood;
    int metaImportanceBad;

    int boardPositionBonus;
};

static constexpr Weights w{
    1000000,   // terminalWin
   -1000000,   // terminalLoss

    25,         // metaOwned
   -25,         // metaOpponentOwned

    400,        // metaTwoInRow
    100,        // metaOneInRow
   -450,        // metaOpponentTwoInRow
   -120,        // metaOpponentOneInRow

    450,        // metaFork
   -550,        // metaOpponentFork

    5,          // subCellControl
   -5,          // subCellOpponentControl

    35,         // subTwoInRow
    8,          // subOneInRow
   -40,         // subOpponentTwoInRow
   -10,         // subOpponentOneInRow

    60,         // subFork
   -70,         // subOpponentFork

    40,         // forcedGood
    80,         // forcedVeryGood
   -50,         // forcedBad
   -120,        // forcedVeryBad

    60,         // freeMove

   -200,        // metaImportanceGood
   -500,        // metaImportanceBad

    3           // boardPositionBonus
};

    static constexpr int boardWeight[9] =
    {
        3,2,3,
        2,4,2,
        3,2,3
    };


private:
    Features extract(const GameState& state) const;

    void extractTerminal(
        const UltimateBoard& b,
        Features& f,
        CellState me,
        CellState opp) const;

    void extractMeta(
        const UltimateBoard& b,
        Features& f,
        CellState me,
        CellState opp) const;

    void extractSubBoards(
        const UltimateBoard& b,
        Features& f,
        CellState me,
        CellState opp) const;

    void extractForcedMoves(
        const UltimateBoard& b,
        Features& f,
        CellState me,
        CellState opp) const;

    int evaluateMetaImportance(
        const UltimateBoard& b,
        int boardIndex,
        CellState me,
        CellState opp,
        Features& f) const;

    int dot(const Features& f) const;
};

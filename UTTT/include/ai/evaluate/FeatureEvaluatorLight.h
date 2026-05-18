#pragma once

#include "core/GameState.h"
#include "ai/evaluate/IEvaluator.h"

class FeatureEvaluatorLight : public IEvaluator
{
public:
    int evaluate(const GameState& state) const override;
    int evaluate(const GameState& state, CellState perspective) const;

private:
    struct LightFeatures
    {
        // Terminal conditions
        int terminalWin = 0;
        int terminalLoss = 0;

        // Meta board features (Very fast lookup)
        int metaOwned = 0;
        int metaOpponentOwned = 0;
        int metaTwoInRow = 0;
        int metaOpponentTwoInRow = 0;

        // Active forced sub-board features (Chirurgical inspection)
        int subCellControl = 0;
        int subCellOpponentControl = 0;
        int subTwoInRow = 0;
        int subOpponentTwoInRow = 0;

        // Macro-board pressure metrics
        int freeMove = 0;
        int metaImportanceGood = 0;
        int metaImportanceBad = 0;
        int boardPositionBonus = 0;
    };

    struct LightWeights
    {
        int terminalWin;
        int terminalLoss;
        int metaOwned;
        int metaOpponentOwned;
        int metaTwoInRow;
        int metaOpponentTwoInRow;
        int subCellControl;
        int subCellOpponentControl;
        int subTwoInRow;
        int subOpponentTwoInRow;
        int freeMove;
        int metaImportanceGood;
        int metaImportanceBad;
        int boardPositionBonus;
    };

    // On reprend scrupuleusement les proportions exactes de tes vrais poids
    static constexpr LightWeights w{
        1000000,   // terminalWin
       -1000000,   // terminalLoss
        1200,      // metaOwned
       -1500,      // metaOpponentOwned
        8000,      // metaTwoInRow
       -15000,     // metaOpponentTwoInRow
        2,         // subCellControl
       -3,         // subCellOpponentControl
        120,       // subTwoInRow
       -180,       // subOpponentTwoInRow
       -1000,      // freeMove
        500,       // metaImportanceGood
       -700,       // metaImportanceBad
        8          // boardPositionBonus
    };

    static constexpr int boardWeight[9] = {
        3, 2, 3,
        2, 4, 2,
        3, 2, 3
    };

private:
    LightFeatures extractLight(const GameState& state) const;
    int dotLight(const LightFeatures& f) const;
};

#include "ai/evaluate/FeatureEvaluatorLight.h"
#include "core/WinPatterns.h"
#include <algorithm>

int FeatureEvaluatorLight::evaluate(const GameState& state) const
{
    LightFeatures f = extractLight(state);
    return dotLight(f);
}

int FeatureEvaluatorLight::evaluate(const GameState& state, CellState perspective) const
{
    // Si tu as besoin d'une évaluation forcée selon une perspective précise
    LightFeatures f = extractLight(state);
    return dotLight(f);
}

FeatureEvaluatorLight::LightFeatures
FeatureEvaluatorLight::extractLight(const GameState& state) const
{
    LightFeatures f;
    const UltimateBoard& b = state.getBoard();

    CellState me = state.getMyPlayer();
    CellState opp = (me == CellState::X) ? CellState::O : CellState::X;

    // 1. Analyse Terminale Immédiate
    CellState winner = b.checkWinner();
    if (winner == me) {
        f.terminalWin = 1;
        return f;
    } else if (winner == opp) {
        f.terminalLoss = 1;
        return f;
    }

    // 2. Extraction du Macro-Board (Meta-game) - Pas de boucle sur les sous-grilles !
    for (int i = 0; i < 9; i++) {
        CellState owner = b.getBoard(i).checkWinner();
        if (owner == me) f.metaOwned += boardWeight[i];
        else if (owner == opp) f.metaOpponentOwned += boardWeight[i];
    }

    // Calcul des alignements macro directs (Menaces à l'échelle du grand plateau)
    for (const auto& line : WIN_LINES) {
        int myCount = 0;
        int oppCount = 0;
        int emptyCount = 0;

        for (int idx : line) {
            CellState owner = b.getBoard(idx).checkWinner();
            if (owner == me) myCount++;
            else if (owner == opp) oppCount++;
            else emptyCount++;
        }

        if (myCount == 2 && emptyCount == 1) f.metaTwoInRow++;
        if (oppCount == 2 && emptyCount == 1) f.metaOpponentTwoInRow++;
    }

    // 3. Extraction ciblée de la sous-grille ACTIVE uniquement
    int activeBoard = b.getActiveBoard();
    if (activeBoard == -1) {
        f.freeMove = 1;
    } else {
        const SubBoard& sb = b.getBoard(activeBoard);

        if (sb.checkWinner() != CellState::EMPTY || sb.isFull()) {
            f.freeMove = 1;
        } else {
            // On calcule l'importance macro de la grille sur laquelle on nous force à jouer
            f.boardPositionBonus -= boardWeight[activeBoard];

            for (const auto& line : WIN_LINES) {
                bool containsBoard = false;
                int myMetaCount = 0;
                int oppMetaCount = 0;

                for (int idx : line) {
                    if (idx == activeBoard) containsBoard = true;
                    else {
                        CellState owner = b.getBoard(idx).checkWinner();
                        if (owner == me) myMetaCount++;
                        else if (owner == opp) oppMetaCount++;
                    }
                }

                if (containsBoard) {
                    if (myMetaCount == 2) f.metaImportanceGood++;
                    if (oppMetaCount == 2) f.metaImportanceBad++;
                }
            }

            // Analyse tactique ultra-rapide des cellules de CETTE sous-grille unique
            for (int i = 0; i < 9; i++) {
                CellState cell = sb.getCell(i).getState();
                if (cell == me) f.subCellControl += boardWeight[i];
                else if (cell == opp) f.subCellOpponentControl += boardWeight[i];
            }

            // Menaces tactiques de la sous-grille active
            for (const auto& line : WIN_LINES) {
                int myCount = 0;
                int oppCount = 0;
                int emptyCount = 0;

                for (int idx : line) {
                    CellState cell = sb.getCell(idx).getState();
                    if (cell == me) myCount++;
                    else if (cell == opp) oppCount++;
                    else emptyCount++;
                }

                if (myCount == 2 && emptyCount == 1) f.subTwoInRow++;
                if (oppCount == 2 && emptyCount == 1) f.subOpponentTwoInRow++;
            }
        }
    }

    return f;
}

int FeatureEvaluatorLight::dotLight(const LightFeatures& f) const
{
    int score = 0;

    score += f.terminalWin * w.terminalWin;
    score += f.terminalLoss * w.terminalLoss;

    score += f.metaOwned * w.metaOwned;
    score += f.metaOpponentOwned * w.metaOpponentOwned;

    score += f.metaTwoInRow * w.metaTwoInRow;
    score += f.metaOpponentTwoInRow * w.metaOpponentTwoInRow;

    score += f.subCellControl * w.subCellControl;
    score += f.subCellOpponentControl * w.subCellOpponentControl;

    score += f.subTwoInRow * w.subTwoInRow;
    score += f.subOpponentTwoInRow * w.subOpponentTwoInRow;

    score += f.freeMove * w.freeMove;

    score += f.metaImportanceGood * w.metaImportanceGood;
    score += f.metaImportanceBad * w.metaImportanceBad;

    score += f.boardPositionBonus * w.boardPositionBonus;

    return score;
}

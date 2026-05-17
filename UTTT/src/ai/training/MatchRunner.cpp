#include "ai/training/MatchRunner.h"
#include "ai/GameManager.h"
#include "ai/training/TrainingFeatureEvaluator.h"
#include <chrono>
#include <iostream>

void applyWeightsToEvaluator(const std::vector<double>& weights, TrainingFeatureEvaluator* evaluator) {
    if (!evaluator || weights.size() < 28) return;

    evaluator->w.terminalWin             = static_cast<int>(weights[0]);
    evaluator->w.terminalLoss            = static_cast<int>(weights[1]);
    evaluator->w.metaOwned               = static_cast<int>(weights[2]);
    evaluator->w.metaOpponentOwned       = static_cast<int>(weights[3]);
    evaluator->w.metaTwoInRow            = static_cast<int>(weights[4]);
    evaluator->w.metaOneInRow            = static_cast<int>(weights[5]);
    evaluator->w.metaOpponentTwoInRow    = static_cast<int>(weights[6]);
    evaluator->w.metaOpponentOneInRow    = static_cast<int>(weights[7]);
    evaluator->w.metaFork                = static_cast<int>(weights[8]);
    evaluator->w.metaOpponentFork        = static_cast<int>(weights[9]);
    evaluator->w.subCellControl          = static_cast<int>(weights[10]);
    evaluator->w.subCellOpponentControl  = static_cast<int>(weights[11]);
    evaluator->w.subTwoInRow             = static_cast<int>(weights[12]);
    evaluator->w.subOneInRow             = static_cast<int>(weights[13]);
    evaluator->w.subOpponentTwoInRow     = static_cast<int>(weights[14]);
    evaluator->w.subOpponentOneInRow     = static_cast<int>(weights[15]);
    evaluator->w.subFork                 = static_cast<int>(weights[16]);
    evaluator->w.subOpponentFork         = static_cast<int>(weights[17]);
    evaluator->w.forcedGood              = static_cast<int>(weights[18]);
    evaluator->w.forcedVeryGood          = static_cast<int>(weights[19]);
    evaluator->w.forcedBad               = static_cast<int>(weights[20]);
    evaluator->w.forcedVeryBad           = static_cast<int>(weights[21]);
    evaluator->w.freeMove                = static_cast<int>(weights[22]);
    evaluator->w.metaImportanceGood      = static_cast<int>(weights[23]);
    evaluator->w.metaImportanceBad       = static_cast<int>(weights[24]);
    evaluator->w.boardPositionBonus      = static_cast<int>(weights[25]);
    evaluator->w.metaNearWin             = static_cast<int>(weights[26]);
    evaluator->w.metaOpponentNearWin     = static_cast<int>(weights[27]);
}

int MatchRunner::runMatch(const std::vector<double>& weightsX,
                          const std::vector<double>& weightsO,
                          DataLogger* logger,
                          int gameId) const
{
    long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    GameManager managerX(timestamp);
    GameManager managerO(timestamp);

    TrainingFeatureEvaluator* featEvalX = static_cast<TrainingFeatureEvaluator*>(managerX.getEvaluator());
    TrainingFeatureEvaluator* featEvalO = static_cast<TrainingFeatureEvaluator*>(managerO.getEvaluator());

    if (!featEvalX || !featEvalO) {
        std::cerr << "CRITICAL ERROR: Manager evaluators are null or not TrainingFeatureEvaluators!" << std::endl;
        return 0;
    }

    applyWeightsToEvaluator(weightsX, featEvalX);
    applyWeightsToEvaluator(weightsO, featEvalO);

    managerX.init(CellState::X);
    managerO.init(CellState::O);

    int moveCount = 0;

    while (!managerX.getState().isTerminal()) {

        if (moveCount > 90) {
            std::cout << "ERROR";
            break;
        }

        CellState currentPlayer = managerX.getState().getCurrentPlayer();
        AIMove move;

        if (currentPlayer == CellState::X) {
            move = managerX.chooseMove();
        } else {
            move = managerO.chooseMove();
        }

        managerX.applyMove(move);
        managerO.applyMove(move);

        moveCount++;
    }

    CellState winner = managerX.getState().getWinner();
    int fitnessResult = 0;

    if (winner == CellState::X){
        fitnessResult = 1;
        std::cout << " WINNER X "<<std::endl;
    }
    else if (winner == CellState::O){
        std::cout << " WINNER O "<<std::endl;
        fitnessResult = -1;
    }
    else{
        fitnessResult = 0;
        std::cout << " NO WINNER "<<std::endl;
    }


    return fitnessResult;
}

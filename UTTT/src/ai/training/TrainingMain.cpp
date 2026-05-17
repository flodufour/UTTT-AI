#include <iostream>
#include <vector>
#include "ai/training/MatchRunner.h"
#include "ai/training/GeneticAlgorithm.h"

int main() {
    std::cout << "Starting Genetic Algorithm Optimization..." << std::endl;

    int numFeatures = 28;
    int populationSize = 40;
    int numGenerations = 50;
    double mutationRate = 0.15;
    double mutationStrength = 15.0;
    double elitismRatio = 0.10;

    MatchRunner runner;
    GeneticAlgorithm ga(populationSize, mutationRate, mutationStrength, elitismRatio);

    // good player for reference
    std::vector<double> currentBestWeights = {
         1000000.0, // [0]  terminalWin
        -1000000.0, // [1]  terminalLoss
         1200.0,    // [2]  metaOwned
        -1500.0,    // [3]  metaOpponentOwned
         8000.0,    // [4]  metaTwoInRow
         1200.0,    // [5]  metaOneInRow
        -15000.0,   // [6]  metaOpponentTwoInRow
        -1800.0,    // [7]  metaOpponentOneInRow
         10000.0,   // [8]  metaFork
        -12000.0,   // [9]  metaOpponentFork
         2.0,       // [10] subCellControl
        -3.0,       // [11] subCellOpponentControl
         120.0,     // [12] subTwoInRow
         20.0,      // [13] subOneInRow
        -180.0,     // [14] subOpponentTwoInRow
        -35.0,      // [15] subOpponentOneInRow
         250.0,     // [16] subFork
        -350.0,     // [17] subOpponentFork
         40.0,      // [18] forcedGood
         180.0,     // [19] forcedVeryGood
        -60.0,      // [20] forcedBad
        -300.0,     // [21] forcedVeryBad
        -1000.0,    // [22] freeMove
         500.0,     // [23] metaImportanceGood
        -700.0,     // [24] metaImportanceBad
         8.0,       // [25] boardPositionBonus
         600.0,     // [26] metaNearWin
        -850.0      // [27] metaOpponentNearWin
    };

    ga.setBaselineWeights(currentBestWeights);
    ga.initializePopulation(numFeatures);

    for (int gen = 1; gen <= numGenerations; ++gen) {
        std::cout << "--- Generation " << gen << " / " << numGenerations << " ---" << std::endl;

        ga.evaluatePopulation(runner);

        const Individual& champion = ga.getBestIndividual();
        std::cout << "Current Best Fitness: " << champion.fitness << std::endl;
        std::cout << "Top Genotype Meta Ownership Weight: " << champion.weights[2] << std::endl;

        ga.evolve();
    }

    ga.evaluatePopulation(runner);
    const Individual& bestAI = ga.getBestIndividual();

    std::cout << "\n=========================================" << std::endl;
    std::cout << "Optimization complete!" << std::endl;
    std::cout << "=========================================" << std::endl;

    // corresponding names to the features
    const std::vector<std::string> weightNames = {
        "terminalWin", "terminalLoss", "metaOwned", "metaOpponentOwned",
        "metaTwoInRow", "metaOneInRow", "metaOpponentTwoInRow", "metaOpponentOneInRow",
        "metaFork", "metaOpponentFork", "subCellControl", "subCellOpponentControl",
        "subTwoInRow", "subOneInRow", "subOpponentTwoInRow", "subOpponentOneInRow",
        "subFork", "subOpponentFork", "forcedGood", "forcedVeryGood",
        "forcedBad", "forcedVeryBad", "freeMove", "metaImportanceGood",
        "metaImportanceBad", "boardPositionBonus", "metaNearWin", "metaOpponentNearWin"
    };

    for (size_t i = 0; i < bestAI.weights.size(); ++i) {
        std::cout << "   " << static_cast<int>(bestAI.weights[i]) << ", // " << weightNames[i] << std::endl;
    }

    return 0;
}

#pragma once
#include <vector>
#include <random>
#include "MatchRunner.h"

struct Individual {
    std::vector<double> weights;
    int fitness = 0;
};

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int populationSize, double mutationRate, double mutationStrength, double elitismRatio);

    void initializePopulation(int numWeights);
    void evaluatePopulation(const MatchRunner& runner);
    void evolve();

    const Individual& getBestIndividual() const;
    void setBaselineWeights(const std::vector<double>& baseline);

private:
    int _populationSize;
    double _mutationRate;
    double _mutationStrength;
    double _elitismRatio;

    std::vector<Individual> _population;
    std::vector<double> _baselineWeights;

    std::mt19937 _rng;

    Individual crossover(const Individual& parentA, const Individual& parentB);
    void mutate(Individual& ind);
    int selectParentIdx();
};

#include "ai/training/GeneticAlgorithm.h"
#include <algorithm>
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm(int populationSize, double mutationRate, double mutationStrength, double elitismRatio)
    : _populationSize(populationSize), _mutationRate(mutationRate),
      _mutationStrength(mutationStrength), _elitismRatio(elitismRatio)
{
    std::random_device rd;
    _rng.seed(rd());
}

void GeneticAlgorithm::setBaselineWeights(const std::vector<double>& baseline) {
    _baselineWeights = baseline;
}

void GeneticAlgorithm::initializePopulation(int numWeights) {
    _population.clear();
    std::uniform_real_distribution<double> dist(-100.0, 100.0);

    for (int i = 0; i < _populationSize; ++i) {
        Individual ind;
        ind.weights.resize(numWeights);
        for (int j = 0; j < numWeights; ++j) {
            ind.weights[j] = dist(_rng);
        }

        if (numWeights > 1) {
            ind.weights[0] = 1000000.0;
            ind.weights[1] = -1000000.0;
        }

        _population.push_back(ind);
    }
}

void GeneticAlgorithm::evaluatePopulation(const MatchRunner& runner) {
    for (auto& ind : _population) {
        ind.fitness = 0;
    }

    for (int i = 0; i < _populationSize; ++i) {

        if (!_baselineWeights.empty()) {
            int resX = runner.runMatch(_population[i].weights, _baselineWeights);
            if (resX == 1)       _population[i].fitness += 10;
            else if (resX == 0)  _population[i].fitness += 2;

            int resO = runner.runMatch(_baselineWeights, _population[i].weights);
            if (resO == -1)      _population[i].fitness += 10;
            else if (resO == 0)  _population[i].fitness += 2;
        }

        for (int step = 1; step <= 3; ++step) {
            int j = (i + step) % _populationSize;

            int result = runner.runMatch(_population[i].weights, _population[j].weights);
            if (result == 1) {
                _population[i].fitness += 3;
            } else if (result == -1) {
                _population[j].fitness += 3;
            } else {
                _population[i].fitness += 1;
                _population[j].fitness += 1;
            }
        }
    }
}

void GeneticAlgorithm::evolve() {
    std::sort(_population.begin(), _population.end(), [](const Individual& a, const Individual& b) {
        return a.fitness > b.fitness;
    });

    std::vector<Individual> nextGeneration;

    int numElites = static_cast<int>(_populationSize * _elitismRatio);
    for (int i = 0; i < numElites; ++i) {
        nextGeneration.push_back(_population[i]);
    }

    while (nextGeneration.size() < static_cast<size_t>(_populationSize)) {
        int parentAIdx = selectParentIdx();
        int parentBIdx = selectParentIdx();

        Individual child = crossover(_population[parentAIdx], _population[parentBIdx]);
        mutate(child);

        nextGeneration.push_back(child);
    }

    _population = std::move(nextGeneration);
}

int GeneticAlgorithm::selectParentIdx() {
    std::uniform_int_distribution<int> dist(0, _populationSize - 1);
    int idx1 = dist(_rng);
    int idx2 = dist(_rng);
    int idx3 = dist(_rng);

    int bestIdx = idx1;
    if (_population[idx2].fitness > _population[bestIdx].fitness) bestIdx = idx2;
    if (_population[idx3].fitness > _population[bestIdx].fitness) bestIdx = idx3;

    return bestIdx;
}

Individual GeneticAlgorithm::crossover(const Individual& parentA, const Individual& parentB) {
    Individual child;
    child.weights.resize(parentA.weights.size());
    std::uniform_int_distribution<int> coinFlip(0, 1);

    for (size_t i = 0; i < parentA.weights.size(); ++i) {
        child.weights[i] = (coinFlip(_rng) == 0) ? parentA.weights[i] : parentB.weights[i];
    }
    return child;
}

void GeneticAlgorithm::mutate(Individual& ind) {
    std::uniform_real_distribution<double> probDist(0.0, 1.0);
    std::normal_distribution<double> gaussDist(0.0, _mutationStrength);

    for (size_t i = 0; i < ind.weights.size(); ++i) {
        if (i == 0 || i == 1) continue;

        if (probDist(_rng) < _mutationRate) {
            ind.weights[i] += gaussDist(_rng);
        }
    }
}

const Individual& GeneticAlgorithm::getBestIndividual() const {
    return _population[0];
}

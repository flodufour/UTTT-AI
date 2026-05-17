# Training Optimization Overview

## Context
Our Minimax AI equipped with the feature evaluator currently plateaus when playing against the "Medium 2" baseline bot. Manual tuning of the weights array proved ineffective and time-consuming due to the interconnected nature of the heuristics in Ultimate Tic-Tac-Toe. To resolve this, we are implementing a Genetic Algorithm (GA) to automatically evolve and optimize the weight configuration.

## Target Directory Structure
The training pipeline resides inside the training/ directory.

```
training/
├── DataLogger.h/cpp      # Logger
├── MatchRunner.h/cpp     # Manages game simulation between two weight configurations
├── GeneticAlgorithm.h/cpp # Core GA logic (selection, crossover, mutation)
└── trainingMain.cpp      # Main entry point for the training execution
```

## The Genetic Algorithm Process

1 - Initialization: Create an initial population of individuals in trainingMain.cpp. Each individual holds a vector representing the evaluation weights.

2 - Evaluation : MatchRunner simulates games between individuals using a low-depth Minimax strategy to ensure speed. Individuals receive fitness points based on performance (e.g., Win: 3 pts, Draw: 1 pt, Loss: 0 pts).

3 - Selection: Sort the population by fitness. Retain the top-performing individuals (elivism) to guarantee the best traits survive.

4 - Crossover: Select pairs of high-scoring parents to produce offspring by randomly mixing their weight vectors.

5 - Mutation: Apply small, random Gaussian modifications to the weights of the new offspring to maintain genetic diversity and discover new strategies.

6 - Iteration: Repeat the evaluation loop for a predefined number of generations until the weights converge.
#pragma once

#include <memory>
#include <vector>
#include <random>
#include <unordered_map>
#include <thread>
#include <mutex>

#include "core/GameState.h"
#include "core/AIMove.h"
#include "ai/evaluate/HeuristicEvaluator.h"
#include "IStrategy.h"

class MCTSStrategy : public IStrategy
{
public:
    explicit MCTSStrategy(int iterations);

    AIMove chooseMove(const GameState& state) override;

private:

    struct Node
    {
        GameState state;
        AIMove move{-1, -1};

        Node* parent = nullptr;
        std::vector<std::unique_ptr<Node>> children;
        std::vector<AIMove> untriedMoves;

        int visits = 0;
        double score = 0.0;

        double prior = 0.0;
        AIMove killerMove{-1, -1};
    };

    using Hash = uint64_t;

private:

    Node* selection(Node* node);
    Node* expansion(Node* node);

    double simulation(GameState state);
    void backpropagation(Node* node, double result);

    double uctValue(Node* node, Node* parent);

    AIMove pickAndRemoveRandom(Node* node);
    AIMove selectBestHeuristicMove(const GameState& state, const std::vector<AIMove>& moves);

    double normalizeResult(const GameState& state);

    double rolloutPolicy(const GameState& state, int depth);
    double noisyHeuristic(const GameState& state);

    double killerHeuristic(Node* node, AIMove move);
    bool isForcedMove(const GameState& state, AIMove& forced);

    Hash computeHash(const GameState& state);

    void worker(int iterations);

private:

    std::unique_ptr<Node> root;

    int _iterations;

    HeuristicEvaluator evaluator;
    std::mt19937 rng;

    CellState rootPlayer = CellState::EMPTY;

    std::unordered_map<Hash, Node*> transpositionTable;
    std::mutex tableMutex;

    int threadCount = std::thread::hardware_concurrency();
};

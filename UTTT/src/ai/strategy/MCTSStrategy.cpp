#include "ai/strategy/MCTSStrategy.h"
#include "utils/ZobristHasher.h"

#include <limits>
#include <random>
#include <algorithm>
#include <iostream>

MCTSStrategy::MCTSStrategy(IEvaluator* evaluator,
                           int iterations,
                           double exploration)
    : _evaluator(evaluator),
      _iterations(iterations),
      _exploration(exploration)
{
}

AIMove MCTSStrategy::chooseMove(GameState& rootState)
{
    Node root(rootState);

    for (int i = 0; i < _iterations; i++)
    {
        Node* node = select(&root);

        if (!node->state.isTerminal() && !node->untriedMoves.empty())
        {
            node = expand(node);
        }

        double result = simulate(node->state);
        backpropagate(node, result);
    }

    Node* best = nullptr;
    int bestVisits = -1;

    for (auto& child : root.children)
    {
        if (child->visits > bestVisits)
        {
            bestVisits = child->visits;
            best = child.get();
        }
    }

    return best ? best->move : AIMove(-1, -1);
}

MCTSStrategy::Node* MCTSStrategy::select(Node* node)
{
    while (!node->state.isTerminal())
    {
        if (!node->untriedMoves.empty())
            return node;

        if (node->children.empty())
            return node;

        Node* best = nullptr;
        double bestScore = -std::numeric_limits<double>::infinity();

        for (auto& child : node->children)
        {
            double score = uctValue(child.get(), node);

            if (score > bestScore)
            {
                bestScore = score;
                best = child.get();
            }
        }

        if (!best)
            return node;

        node = best;
    }

    return node;
}

MCTSStrategy::Node* MCTSStrategy::expand(Node* node)
{
    if (node->untriedMoves.empty())
        return node;

    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, (int)node->untriedMoves.size() - 1);

    int idx = dist(rng);
    AIMove move = node->untriedMoves[idx];

    node->untriedMoves.erase(node->untriedMoves.begin() + idx);

    GameState nextState = node->state;

    if (!nextState.applyMove(move))
        return node;

    node->children.push_back(
        std::make_unique<Node>(nextState, node, move)
    );

    return node->children.back().get();
}

double MCTSStrategy::simulate(GameState state)
{
    CellState rootPlayer = state.getMyPlayer();
    int depth = 0;
    int movesLeft = state.getMovesLeft();

    int maxDepth;
    if (movesLeft > 60)
        maxDepth = 20;
    else if (movesLeft > 40)
        maxDepth = 25;
    else if (movesLeft > 20)
        maxDepth = 30;
    else
        maxDepth = 35;

    static thread_local std::mt19937 rng(std::random_device{}());

    while (!state.isTerminal() && depth < maxDepth)
    {
        auto moves = state.getValidMoves();
        if (moves.empty())
            break;

        AIMove move;

        std::uniform_int_distribution<int> prob(0, 9);
        if (prob(rng) < 8)
        {
            move = selectRolloutMove(state);
        }
        else
        {
            std::uniform_int_distribution<int> dist(0, (int)moves.size() - 1);
            move = moves[dist(rng)];
        }

        if (!state.applyMove(move))
            break;

        depth++;
    }

    CellState winner = state.getWinner();

    if (winner == rootPlayer)
        return 1.0;
    if (winner != CellState::EMPTY)
        return -1.0;

    int score = _evaluator->evaluate(state);

    return std::tanh(score / 1000.0);
}

double MCTSStrategy::uctValue(Node* node, Node* parent) const
{
    if (node->visits == 0)
        return std::numeric_limits<double>::infinity();

    double exploitation = node->value / node->visits;

    double c;
    if (parent->visits < 1000)
        c = 2.5;
    else if (parent->visits < 5000)
        c = 1.8;
    else
        c = 1.2;

    double exploration = c *
        std::sqrt(std::log(parent->visits + 1) / node->visits);

    return exploitation + exploration;
}

uint64_t MCTSStrategy::getHash(const GameState& state) const
{
    return state.getHash();
}

AIMove MCTSStrategy::selectRolloutMove(GameState& state)
{
    auto moves = state.getValidMoves();

    struct ScoredMove {
        AIMove move;
        int score;
    };

    std::vector<ScoredMove> scored;
    scored.reserve(moves.size());

    for (const auto& move : moves)
    {
        GameState tmp = state;
        if (!tmp.applyMove(move))
            continue;

            int score = _evaluator->evaluate(tmp);


        scored.push_back({move, score});
    }

    if (scored.empty())
        return moves[0];

    std::sort(scored.begin(), scored.end(),
              [](auto& a, auto& b) {
                  return a.score > b.score;
              });

    int K = std::min(3, (int)scored.size());

    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, K - 1);

    return scored[dist(rng)].move;
}

void MCTSStrategy::backpropagate(Node* node, double result)
{
    while (node)
    {
        node->visits++;

        node->value += result;

        node = node->parent;
    }
}

#include "ai/strategy/MCTSStrategy.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <thread>
#include <chrono>

// ------------------------------------------------------------

MCTSStrategy::MCTSStrategy(int iterations)
    : _iterations(iterations),
      rng(std::random_device{}())
{}

// ------------------------------------------------------------

AIMove MCTSStrategy::chooseMove(const GameState& state)
{
    auto moves = state.getValidMoves();
    if (moves.empty())
        return {-1, -1};

    rootPlayer = state.getCurrentPlayer();

    root = std::make_unique<Node>();
    root->state = state;
    root->untriedMoves = moves;

    for (int t = 0; t < threadCount; t++)
    {
        worker(_iterations / threadCount);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(15));

    Node* best = nullptr;
    double bestScore = -1e18;

    for (auto& child : root->children)
    {
        double score = (child->visits > 0)
            ? child->score / child->visits
            : -1e18;

        if (score > bestScore)
        {
            bestScore = score;
            best = child.get();
        }
    }

    return best ? best->move : moves[0];
}

// ------------------------------------------------------------

void MCTSStrategy::worker(int iterations)
{
    for (int i = 0; i < iterations; i++)
    {
        Node* node = selection(root.get());
        if (!node) continue;

        Node* expanded = expansion(node);
        if (!expanded) continue;

        double result = simulation(expanded->state);
        backpropagation(expanded, result);
    }
}

// ------------------------------------------------------------

MCTSStrategy::Node* MCTSStrategy::selection(Node* node)
{
    while (true)
    {
        if (node->state.isTerminal())
            return node;

        if (!node->untriedMoves.empty())
            return node;

        Node* best = nullptr;
        double bestVal = -1e18;

        for (auto& child : node->children)
        {
            double val = uctValue(child.get(), node);
            val += killerHeuristic(node, child->move);

            if (val > bestVal)
            {
                bestVal = val;
                best = child.get();
            }
        }

        if (!best)
            return node;

        node = best;
    }
}

// ------------------------------------------------------------

MCTSStrategy::Node* MCTSStrategy::expansion(Node* node)
{
    if (node->untriedMoves.empty())
        return node;

    AIMove move = pickAndRemoveRandom(node);

    auto child = std::make_unique<Node>();
    child->state = node->state;
    child->move = move;
    child->parent = node;

    if (!child->state.applyMove(move))
        return nullptr;

    child->untriedMoves = child->state.getValidMoves();
    child->prior = noisyHeuristic(child->state);

    node->children.push_back(std::move(child));

    return node->children.back().get();
}

// ------------------------------------------------------------

double MCTSStrategy::simulation(GameState state)
{
    const int maxDepth = 80;
    int depth = 0;

    while (!state.isTerminal() && depth < maxDepth)
    {
        auto moves = state.getValidMoves();
        if (moves.empty())
            break;

        AIMove move = selectBestHeuristicMove(state, moves);

        if (!state.applyMove(move))
            break;

        depth++;
    }

    return rolloutPolicy(state, depth);
}

// ------------------------------------------------------------
// UTTT SAFE HASH (SIMPLE MAIS COMPATIBLE)

MCTSStrategy::Hash MCTSStrategy::computeHash(const GameState& state)
{
    Hash h = 0;

    const UltimateBoard& board = state.getBoard();

    for (int b = 0; b < 9; b++)
    {
        const SubBoard& sb = board.getBoard(b);

        for (int c = 0; c < 9; c++)
        {
            // état simplifié mais stable
            if (!sb.getCell(c).isEmpty())
            {
                h ^= std::hash<int>()(b * 9 + c);
            }
        }
    }

    h ^= std::hash<int>()(board.getActiveBoard());

    return h;
}

// ------------------------------------------------------------

bool MCTSStrategy::isForcedMove(const GameState& state, AIMove& forced)
{
    auto moves = state.getValidMoves();

    if (moves.size() == 1)
    {
        forced = moves[0];
        return true;
    }

    return false;
}

// ------------------------------------------------------------

AIMove MCTSStrategy::selectBestHeuristicMove(
    const GameState& state,
    const std::vector<AIMove>& moves)
{
    AIMove best = moves[0];
    double bestScore = -1e18;

    for (auto& m : moves)
    {
        GameState copy = state;

        if (!copy.applyMove(m))
            continue;

        double score = evaluator.evaluate(copy);

        if (copy.isTerminal())
        {
            if (copy.getWinner() == rootPlayer)
                score += 1e6;
            else if (copy.getWinner() != CellState::EMPTY)
                score -= 1e6;
        }

        if (score > bestScore)
        {
            bestScore = score;
            best = m;
        }
    }

    return best;
}

// ------------------------------------------------------------

double MCTSStrategy::uctValue(Node* node, Node* parent)
{
    if (node->visits == 0)
        return 1e9;

    double Q = node->score / node->visits;

    double U = std::sqrt(std::log(parent->visits + 1.0) / node->visits);

    return Q + 1.7 * U;
}

// ------------------------------------------------------------

double MCTSStrategy::killerHeuristic(Node* node, AIMove move)
{
    return (node->killerMove == move) ? 50.0 : 0.0;
}

// ------------------------------------------------------------

double MCTSStrategy::noisyHeuristic(const GameState& state)
{
    std::uniform_real_distribution<double> noise(-25, 25);
    return evaluator.evaluate(state) + noise(rng);
}

// ------------------------------------------------------------

double MCTSStrategy::rolloutPolicy(const GameState& state, int depth)
{
    double base = normalizeResult(state);

    if (!state.isTerminal())
        base += evaluator.evaluate(state) / 120000.0;

    return base * (1.0 - depth * 0.012);
}

// ------------------------------------------------------------

void MCTSStrategy::backpropagation(Node* node, double result)
{
    while (node)
    {
        node->visits++;
        node->score += result;
        result = -result;
        node = node->parent;
    }
}

// ------------------------------------------------------------

AIMove MCTSStrategy::pickAndRemoveRandom(Node* node)
{
    std::uniform_int_distribution<int> dist(0, node->untriedMoves.size() - 1);

    int i = dist(rng);
    AIMove m = node->untriedMoves[i];

    node->untriedMoves[i] = node->untriedMoves.back();
    node->untriedMoves.pop_back();

    return m;
}

// ------------------------------------------------------------

double MCTSStrategy::normalizeResult(const GameState& state)
{
    if (!state.isTerminal())
        return 0.0;

    auto w = state.getWinner();

    if (w == CellState::EMPTY)
        return 0.0;

    return (w == rootPlayer) ? 1.0 : -1.0;
}

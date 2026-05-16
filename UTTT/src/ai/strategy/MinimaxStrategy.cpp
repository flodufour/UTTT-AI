#include "ai/strategy/MinimaxStrategy.h"
#include <limits>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <ctime>
#include <chrono>



MinimaxStrategy::MinimaxStrategy(IEvaluator* evaluator, int depth)
    : _evaluator(evaluator), _maxDepth(depth)
{
    _transpositionTable.resize(TT_SIZE);
}

AIMove MinimaxStrategy::chooseMove(GameState& state) {

    auto start = std::chrono::high_resolution_clock::now();
    AIMove globalBestMove;

    for (int d = 1; d <= _maxDepth && (std::chrono::high_resolution_clock::now() - start) < std::chrono::milliseconds(200) ; ++d) {

    std::cout << d <<std::endl;
        int alpha = -9999999;
        int beta  =  9999999;

        auto moves = state.getValidMoves();

        uint64_t h = state.getHash();
        TTEntry& entry = _transpositionTable[h & (TT_SIZE - 1)];

        if (entry.key == h) {
            AIMove hint = entry.bestMove;
            auto it = std::find(moves.begin(), moves.end(), hint);
            if (it != moves.end()) std::iter_swap(moves.begin(), it);
        }

        AIMove bestMoveIteration;
        int bestScoreIteration = -9999999;

        for (const auto& move : moves) {
            auto undo = state.applyMoveFast(move);
            int score = minimax(state, d - 1, false, alpha, beta);
            state.undoMove(undo);

            if (score > bestScoreIteration) {
                bestScoreIteration = score;
                bestMoveIteration = move;
            }
            alpha = std::max(alpha, bestScoreIteration);
        }

        globalBestMove = bestMoveIteration;

        if (bestScoreIteration > 800000) break;
    }

    return globalBestMove;
}

int MinimaxStrategy::minimax(GameState& state, int depth, bool maximizing, int alpha, int beta)
{
    uint64_t hash = state.getHash();
    int alphaOrig = alpha;

    TTEntry& entry = _transpositionTable[hash & (TT_SIZE - 1)];

    if (entry.key == hash && entry.depth >= depth) {
        if (entry.flag == TTFlag::EXACT) return entry.value;
        else if (entry.flag == TTFlag::LOWER_BOUND) alpha = std::max(alpha, entry.value);
        else if (entry.flag == TTFlag::UPPER_BOUND) beta = std::min(beta, entry.value);

        if (alpha >= beta) return entry.value;
    }

    if (state.isTerminal() || depth == 0)
        return _evaluator->evaluate(state);

    auto moves = state.getValidMoves();
    if (moves.empty()) return _evaluator->evaluate(state);

    if (entry.key == hash) {
        auto it = std::find(moves.begin(), moves.end(), entry.bestMove);
        if (it != moves.end()) std::iter_swap(moves.begin(), it);
    }

    int best;
    AIMove bestMoveLocal;

    if (maximizing) {
        best = -9999999;
        for (const auto& move : moves) {
            auto undo = state.applyMoveFast(move);
            int score = minimax(state, depth - 1, false, alpha, beta);
            state.undoMove(undo);

            if (score > best) {
                best = score;
                bestMoveLocal = move;
            }
            alpha = std::max(alpha, best);
            if (beta <= alpha) break;
        }
    } else {
        best = 9999999;
        for (const auto& move : moves) {
            auto undo = state.applyMoveFast(move);
            int score = minimax(state, depth - 1, true, alpha, beta);
            state.undoMove(undo);

            if (score < best) {
                best = score;
                bestMoveLocal = move;
            }
            beta = std::min(beta, best);
            if (beta <= alpha) break;
        }
    }

    if (entry.key != hash || depth >= entry.depth) {
        entry.key = hash; // Très important !
        entry.value = best;
        entry.depth = depth;
        entry.bestMove = bestMoveLocal;

        if (best <= alphaOrig) entry.flag = TTFlag::UPPER_BOUND;
        else if (best >= beta) entry.flag = TTFlag::LOWER_BOUND;
        else entry.flag = TTFlag::EXACT;
    }

    return best;
}

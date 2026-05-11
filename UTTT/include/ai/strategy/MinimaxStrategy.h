#pragma once

#include "core/AIMove.h"
#include "core/GameState.h"
#include "ai/strategy/IStrategy.h"
#include "ai/evaluate/IEvaluator.h"

#include <unordered_map>
#include <cstdint>

class MinimaxStrategy : public IStrategy {
public:
    // 1. Définition des types de nœuds pour l'élagage Alpha-Beta
    enum class TTFlag { EXACT, LOWER_BOUND, UPPER_BOUND };

    // 2. Structure de la table de transposition
    struct TTEntry {
        int value;      // Score de l'évaluation
        int depth;      // Profondeur de recherche
        TTFlag flag;    // Type de valeur (Exacte, Alpha ou Beta)
        AIMove bestMove;// Le meilleur coup trouvé à cette position
    };

    MinimaxStrategy(IEvaluator* evaluator, int depth = 3);

    AIMove chooseMove(GameState& state) override;

private:
    int minimax(
        GameState& state,
        int depth,
        bool maximizing,
        int alpha,
        int beta
    );

    IEvaluator* _evaluator;
    int _maxDepth;

    // 3. La table de transposition (clé = hash Zobrist)
    std::unordered_map<uint64_t, TTEntry> _transpositionTable;
};

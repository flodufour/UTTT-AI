#pragma once
#include "core/GameState.h"

/// @interface IEvaluator
/// @brief Engine-safe evaluation interface (stateless)
///
/// Contract:
/// - MUST NOT modify GameState
/// - MUST be deterministic
/// - MUST be side-effect free
///
/// Convention:
/// - positive = good for current player
/// - negative = good for opponent

class IEvaluator {
public:

    /// @brief Evaluate a game state (read-only)
    /// @param state current game state (MUST NOT be modified)
    /// @return heuristic score (relative to current player)
    virtual int evaluate(const GameState& state) const = 0;

    virtual ~IEvaluator() = default;
};

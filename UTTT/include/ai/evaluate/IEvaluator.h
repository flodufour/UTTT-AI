#pragma once
#include "core/GameState.h"

/// @interface IEvaluator
/// @brief Engine-safe evaluation interface
///
/// Contract:
/// - MUST NOT modify GameState
/// - MUST be deterministic
///
/// Convention:
/// - positive = good for our player
/// - negative = good for opponent

class IEvaluator {
public:

    /// @brief Evaluate a game state (read-only)
    /// @param state current game state
    /// @return heuristic score (relative to the root player)
    virtual int evaluate(const GameState& state) const = 0;

    virtual ~IEvaluator() = default;
};

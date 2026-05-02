#pragma once


/// @interface IEvaluator
/// @brief Interface for game state evaluation
///
/// Defines a contract for evaluating a GameState and returning
/// a numerical score representing the position quality.

class IEvaluator {
public:

    /// @brief Evaluates the given game state
    /// @param state current game state
    /// @return evaluation score (higher = better position)

    virtual int evaluate(const GameState& state) = 0 ;
    virtual ~IEvaluator() = default;

};

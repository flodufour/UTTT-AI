#include "core/GameState.h"
#include <iostream>

GameState::GameState()
{
    reset();
}

void GameState::reset()
{
    board.reset();
}

void GameState::applyOpponentMove(const GameMove& move)
{
    board.playMove(move.row, move.col, opponent);
}

void GameState::applyMyMove(const GameMove& move)
{
    board.playMove(move.row, move.col, myPlayer);
}

const UltimateBoard& GameState::getBoard() const
{
    return board;
}

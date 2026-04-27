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

void GameState::setPlayers(CellState me)
{
    myPlayer = me;
    opponent = (me == CellState::X) ? CellState::O : CellState::X;

    currentPlayer = CellState::X;
}

void GameState::applyMove(const GameMove& move)
{
    board.playMove(move.row, move.col, currentPlayer);
}

void GameState::switchPlayers()
{
    currentPlayer = (currentPlayer == CellState::X)
                    ? CellState::O
                    : CellState::X;
}

CellState GameState::getCurrentPlayer() const
{
    return currentPlayer;
}

const UltimateBoard& GameState::getBoard() const
{
    return board;
}

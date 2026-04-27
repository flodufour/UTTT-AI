#include "manager/GameManager.h"

GameManager::GameManager()
{
}

void GameManager::init(CellState mySide)
{
    me = mySide;
    opponent = (mySide == CellState::X) ? CellState::O : CellState::X;

    state.reset();
    state.setPlayers(mySide);
}

void GameManager::applyMove(const GameMove& move)
{
    state.applyMove(move);
    state.switchPlayers();
}

GameMove GameManager::chooseMove()
{
    //#TODO
    auto moves = state.getValidMoves();

    if (!moves.empty())
        return moves[0];

    return {-1, -1};
}

const GameState& GameManager::getState() const
{
    return state;
}

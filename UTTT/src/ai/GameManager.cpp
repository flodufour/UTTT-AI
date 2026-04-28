#include "ai/GameManager.h"
#include "ai/strategy/RandomStrategy.h"
#include <iostream>

GameManager::GameManager()
{
    _strategy = new RandomStrategy();
}

void GameManager::init(CellState mySide)
{
    _me = mySide;
    _opponent = (mySide == CellState::X) ? CellState::O : CellState::X;

    _state.reset();
    _state.setPlayers(mySide);
}

void GameManager::applyMove(const AIMove& move)
{
    _state.applyMove(move);
    _state.switchPlayers();
}

AIMove GameManager::chooseMove()
{
    return _strategy->chooseMove(_state);
}

const GameState& GameManager::getState() const
{
    return _state;
}

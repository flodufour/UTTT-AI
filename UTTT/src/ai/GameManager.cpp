#include "ai/GameManager.h"
#include "ai/strategy/RandomStrategy.h"
#include <iostream>

GameManager::GameManager()
{
    strategy = new RandomStrategy();
}

void GameManager::init(CellState mySide)
{
    me = mySide;
    opponent = (mySide == CellState::X) ? CellState::O : CellState::X;

    state.reset();
    state.setPlayers(mySide);
}

void GameManager::applyMove(const AIMove& move)
{
    state.applyMove(move);
    state.switchPlayers();
}

AIMove GameManager::chooseMove()
{
    return strategy->chooseMove(state);
}

const GameState& GameManager::getState() const
{
    return state;
}

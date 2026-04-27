#include "manager/GameManager.h"
#include <iostream>

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

void GameManager::applyMove(const AIMove& move)
{
    state.applyMove(move);
    state.switchPlayers();
}

AIMove GameManager::chooseMove()
{
    //#TODO
    auto moves = state.getValidMoves();

    for (auto move : moves){
        std::cout << move.boardIndex << " " << move.cellIndex << std::endl;
    }

    if (!moves.empty())
        return moves[0];

    return {-1, -1};
}

const GameState& GameManager::getState() const
{
    return state;
}

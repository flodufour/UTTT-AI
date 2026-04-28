# Ultimate Tic Tac Toe AI

This project is an AI implementation for Ultimate Tic Tac Toe (UTTT).  
It is designed with a modular architecture to easily test and compare different strategies (Greedy, Random, Heuristic, Minimax).

---

# Project Structure

```
src/
├── core/
│   ├── GameState.h / GameState.cpp
│   ├── UltimateBoard.h / UltimateBoard.cpp
│   ├── SubBoard.h / SubBoard.cpp
│
├── ai/
│   ├── GameManager.h / GameManager.cpp
│
│   ├── evaluate/
│   │   ├── Evaluation.h / Evaluation.cpp
│   │
│   ├── strategy/
│   │   ├── IStrategy.h
│   │   ├── GreedyStrategy.h / GreedyStrategy.cpp  TODO
│   │   ├── RandomStrategy.h / RandomStrategy.cpp
│   │   ├── HeuristicStrategy.h / HeuristicStrategy.cpp TODO
│   │   ├── MinimaxStrategy.h / MinimaxStrategy.cpp TODO
│
├── utils/
│   ├── MoveConverter.h / MoveConverter.cpp
│
├── main.cpp
```

# Architecture

## Core

Implements the game logic:

- GameState: full game representation
- UltimateBoard: 9 subboards management
- SubBoard: standard 3x3 tic tac toe logic

## AI

### GameManager

Handles:

- current game state
- applying moves
- delegating decision to a strategy

## Strategy System

All AIs implement:

class IStrategy {
public:
    virtual AIMove chooseMove(const GameState& state) = 0;
    virtual ~IStrategy() = default;
};

Available strategies:

- RandomStrategy: random valid moves

## Evaluation


## Game Flow

Engine → GameManager → Strategy → GameState → Move

- Engine provides opponent move
- GameState is updated
- Strategy selects best move
- Move is returned to engine
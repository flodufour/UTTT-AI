# Ultimate Tic Tac Toe AI

AI implementation for Ultimate Tic Tac Toe featuring advanced search algorithms and modular evaluation systems.

---

## Project Structure

```text
src/
├── core/
│   ├── GameState.h / .cpp          (Full state, Undo system, Zobrist Hashing)
│   ├── UltimateBoard.h / .cpp      (9x9 board orchestration)
│   ├── SubBoard.h / .cpp           (Local 3x3 logic and win detection)
│   └── Cell.h                      (Cell state representation)
│
├── ai/
│   ├── GameManager.h / .cpp        (Game session orchestration)
│   ├── ArenaHost.h / .cpp          (Engine protocol bridge)
│   │
│   ├── strategy/
│   │   ├── IStrategy.h             (Strategy interface)
│   │   ├── MinimaxStrategy.h       (Alpha-Beta with Iterative Deepening)
│   │   ├── MCTSStrategy.h          (Monte Carlo Tree Search)
│   │   ├── SimpleStrategy.h        (Rule-based decision making)
│   │   └── RandomStrategy.h        (Baseline random moves)
│   │
│   ├── evaluate/
│   │   ├── IEvaluator.h            (Evaluation interface)
│   │   ├── FeatureEvaluator.h      (Linear weight-based evaluation)
│   │   └── HeuristicEvaluator.h    (Pattern-based evaluation)
│   │
│   └── training/
│       └── DataLogger.h / .cpp     (Dataset generation for ML)
│
├── utils/
│   ├── MoveConverter.h / .cpp      (Coordinate transformations)
│   ├── ZobristHasher.h / .cpp      (Random keys for hashing)
│   └── MoveUndo.h                  (Structure for fast search rollback)
│
└── main.cpp                        (Entry point)
```

---

## Architecture

### Core Logic

- **GameState**: Manages the global state, turn transitions, and provides an optimized undo system for deep tree searches.
- **Bitwise Logic**: Sub-boards are managed efficiently to allow high nodes-per-second during search.
- **Zobrist Hashing**: Implements incremental hashing to support Transposition Tables.

---

## AI System

### Strategy Interface

All decision-making components implement the following interface:

```code 
class IStrategy {
public:
virtual AIMove chooseMove(GameState& state) = 0;
virtual void reset() = 0;
virtual ~IStrategy() = default;
}; 
```


### Minimax Strategy

The primary competitive engine uses:

- **Alpha-Beta Pruning**: Eliminates suboptimal branches.
- **Iterative Deepening**: Returns the best move found within a specific time limit (e.g., 2000ms).
- **Transposition Table**: Fixed-size array-based hash map (1,048,576 entries) with depth-based replacement.
- **Move Ordering**: Uses Transposition Table hints to explore promising moves first, maximizing pruning efficiency.

### Evaluation (FeatureEvaluator)

The AI evaluates positions based on a linear combination of features:

- Terminal states (Wins/Losses)
- Sub-board control and Meta-board alignment
- Local and global threats (Two-in-a-row)
- Center control and forks

### Communication (ArenaHost)

The ArenaHost class decouples the AI logic from the game engine protocol:

- Handles the high-level loop for multiple game sessions
- Detects player side (X or O) based on the initial move sequence
- Synchronizes the GameManager state with external engine moves

---

## Technical Optimizations

- **Memory Management**: Pre-allocated fixed-size Transposition Tables to avoid allocations during search.
- **Fast Undo**: `applyMoveFast` and `undoMove` modify a single GameState object without expensive deep copying.
- **Performance**: High nodes-per-second by minimizing branch mispredictions and memory redirections.

---

## Game Flow

1. **Main**: Initializes Zobrist keys and instantiates GameManager and ArenaHost.
2. **ArenaHost**: Receives a move from the engine.
3. **GameManager**: Updates the internal GameState.
4. **Strategy**: Minimax explores the search space using the Evaluator.
5. **Decision**: Returns the best move via ArenaHost to the engine.
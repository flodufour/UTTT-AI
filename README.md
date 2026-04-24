# LouisTacFlo

## SKELETON

UTTT_Project/
│
├── UTTT_Template.cbp   
├── libUTTTLib.a       
├── main.cpp  
├── main.h
├── images/
│
├── src/
│   ├── core/
│   │   ├── Board.cpp
│   │   ├── Game.cpp
│   │   ├── Move.cpp
│   │
│   ├── ai/
│   │   ├── AI.cpp
│   │   ├── Minimax.cpp
│   │   ├── Heuristic.cpp
│   │
│   ├── interface/
│   │   ├── Adapter.cpp   ← lien avec la lib
│
│   └── utils/
│       ├── Timer.cpp
│
├── include/
│   ├── core/
│   │   ├── Board.h
│   │   ├── Game.h
│   │   ├── Move.h
│   │
│   ├── ai/
│   │   ├── AI.h
│   │   ├── Minimax.h
│   │   ├── Heuristic.h
│   │
│   ├── interface/
│   │   ├── Adapter.h
│   │
│   └── utils/
│       ├── Timer.h
│
└── docs/
    ├── architecture.md
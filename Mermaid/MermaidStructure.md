flowchart TD

A[main.cpp] --> B[Adapter]

B --> C[LibUTTTLib.a]

A --> D[AI]

D --> E[Minimax]
D --> F[Heuristic]
D --> K[MoveGenerator]

E --> G[UltimateBoard]
F --> G
K --> G

G --> I[SubBoard]
I --> H[Cell]

G --> H[Move]
I --> H

B --> G
B --> H

G --> J[Game Logic / Rules]

D --> L[Timer]

subgraph CORE
G
I
H
J
end

subgraph AI
D
E
F
K
end

subgraph INTERFACE
B
end

subgraph UTILS
L
end

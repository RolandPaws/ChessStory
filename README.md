# Chess Roguelike

A C++ roguelike-fantasy fusion built on a chess foundation. Standard chess pieces
are joined by unique fantasy classes, a dynamic board, abilities, items, and
progression — all mounted on a chess grid.

## Concept

- **Board**: Larger than standard chess (12×12 by default). Tiles have types
  (Fire, Ice, Healing, Void, etc.) that evolve over time.
- **Pieces**: 20 piece types — the classic 6 plus 14 extended roguelike classes
  (Mage, Archer, Assassin, Necromancer, Valkyrie, Phantom, Golem, etc.).
- **Movement**: Classical patterns (sliding, leaping, stepping) + custom
  patterns for unique classes. The movement system validates all legal moves
  considering board obstacles, friendly/enemy pieces, and tile passability.
- **Combat**: Attack vs. defence damage calculation, critical hits, ranged/melee,
  tile modifiers, thorns, and ability triggers.
- **Progression**: Pieces gain XP, level up, learn abilities, equip items, and
  gain passive skills.
- **Board Evolution**: Tiles spread, melt, grow, and mutate over time.
  Environmental events (earthquakes, random tile growth) keep the battlefield
  dynamic.

## Getting Started

### Prerequisites

- C++20 compatible compiler (MSVC 2022+, GCC 11+, Clang 14+)
- CMake 3.20+

### Build

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Run

```bash
# Windows
build\Release\chessroguelike.exe

# Linux / macOS
./build/chessroguelike
```

### Run Tests

```bash
cd build
ctest --output-on-failure
# or directly:
./tests/Release/chessroguelike_tests.exe
```

## Project Structure

```
D:\GAME\
├── CMakeLists.txt           # Root build config
├── src/                     # Game source
│   ├── main.cpp             # Entry point
│   ├── core/                # Core engine
│   │   ├── Game.h/.cpp      # Game orchestrator
│   │   ├── Board.h/.cpp     # Board grid + piece management
│   │   ├── Tile.h/.cpp      # Single cell on the board
│   │   └── TileType.h       # Tile type definitions (15 types)
│   ├── pieces/              # Piece system
│   │   ├── Piece.h/.cpp     # Base piece class
│   │   ├── PieceType.h      # 20 piece type definitions
│   │   └── Movement.h/.cpp  # Movement patterns + validation
│   ├── player/              # Roguelike player systems
│   │   ├── Player.h/.cpp    # Player with army + resources
│   │   ├── Ability.h/.cpp   # Active abilities with cooldowns
│   │   ├── Skill.h/.cpp     # Passive skills
│   │   └── Inventory.h/.cpp # Item/inventory system
│   ├── systems/             # Game systems
│   │   ├── TurnSystem.h/.cpp
│   │   ├── CombatSystem.h/.cpp
│   │   └── BoardEvolution.h/.cpp
│   └── utils/
│       └── Vector2D.h       # 2D coordinate utility
├── tests/                   # Unit tests
│   ├── test_board.cpp
│   ├── test_pieces.cpp
│   └── test_movement.cpp
└── build/                   # Build output (gitignored)
```

## Piece Types

| # | Piece | Movement | Special |
|---|-------|----------|---------|
| 1 | King | 1 step any dir | Must be protected |
| 2 | Queen | Sliding any dir | Versatile powerhouse |
| 3 | Rook | Sliding cardinal | Straight-line control |
| 4 | Bishop | Sliding diagonal | Diagonal specialist |
| 5 | Knight | L-shaped leap | Jumps over pieces |
| 6 | Pawn | 1 step forward | Can promote |
| 7 | Archer | 1 step + ranged | Ranged attack 2-3 tiles |
| 8 | Mage | 1 step cardinal | AoE + tile conversion |
| 9 | Paladin | 1 step any dir | Heals allies, tanky |
| 10 | Assassin | 2 steps | Teleport, backstab |
| 11 | Warlock | 1 step cardinal | Curses, lifesteal |
| 12 | Druid | 1 step cardinal | Changes tile types |
| 13 | Berserker | 1 step any dir | Stronger when hurt |
| 14 | Necromancer | 1 step cardinal | Summons skeletons |
| 15 | Valkyrie | Fly (range 3) | Resurrects once |
| 16 | Engineer | 1 step cardinal | Places walls/traps |
| 17 | Golem | 1 step cardinal | Slow, durable, push immune |
| 18 | Phantom | 1 step any dir | Phases through walls |
| 19 | Mimic | 1 step any dir | Copies enemy patterns |
| 20 | Chronomancer | 1 step any dir | Rewind, extra turns |

## Tile Types

- Normal, Wall, Water, Fire, Ice, Lava, Thorn, Healing, ManaWell,
  Void, Shield, Warp, Cursed, Empowered, Temporal

## License

MIT

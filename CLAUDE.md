# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Gomoku is a two-player strategy game with capture mechanics and AI opponent. Players alternate placing stones on a 19x19 board, aiming to either create an unbroken line of 5+ stones or capture 10 opponent stones (5 pairs).

Key game mechanics:
- Win by alignment (5+ in a row) or by capturing 10 opponent stones
- Stones captured by flanking: exactly 2 opponent stones between your stones (e.g., X O O X → X _ _ X)
- Double-Free-Three Ban: cannot create two open three-in-a-rows simultaneously
- Breaking five-stone lines through capture is possible until line becomes unbreakable

## Build and Run Commands

### Building
```bash
# Quick build using script
./make.sh

# Or manually with CMake
cmake -S . -B build
cmake --build build
```

### Running
```bash
# Build and run
./run.sh

# Or manually
./build/Gomoku
```

### Testing
```bash
# Run all tests
./ctest.sh

# Or manually
cd build && ctest

# Run with verbose output
cd build && ./GomokuTests -s --reporter console
```

## Architecture

### Core Components

**Board System** (`src/board/Board.cpp`, `include/board/Board.h`)
- Uses bitmask representation (`StoneMask` = `std::array<uint32_t, 19>`) for efficient 19x19 grid storage
- Each row stored as 32-bit integer, stones represented as bits
- Maintains separate grids for white/black stones
- Handles save/restore state for AI search tree exploration
- Tracks captured stone counts for both players

**AI System** (`src/minmax/MinMax.cpp`, `include/minmax/MinMax.hpp`)
- Negamax algorithm with alpha-beta pruning
- Configurable search depth via `MinMax::MAX_DEPTH`
- Move generation filters positions near existing stones to reduce search space
- Generates decision tree JSON for debugging/analysis
- Uses `HeuristicService` for position evaluation

**Display System** (`src/display/DisplayService.cpp`)
- Scene-based architecture using SFML for rendering
- Available scenes: `MainMenuScene`, `BoardScene`, `PvPScene`, `PvEScene`
- Scene switching via `DisplayService::changeScene()`
- Keyboard event handling for game controls

### Game Logic Services

**CheckWinService** - Detects 5+ stone alignments in all directions

**CaptureService** - Resolves stone captures when flanking patterns (X O O X) are detected

**CheckLegalMove** - Validates moves:
- Position bounds checking
- Occupied square detection
- Double-free-three detection (forbidden move pattern)
- Integration with capture checking

**AlignmentChecker** - Analyzes stone patterns and alignment lengths in all 4 directions (horizontal, vertical, both diagonals)

**HeuristicService** - Evaluates board positions for AI:
- Win detection: 1,000,000 points
- Capture completion: 8,000 points per capture
- Capture threats: 2,000 points
- Open patterns: 5,000 (four), 1,000 (three), 100 (two)
- Evaluates both immediate threats and potential patterns

**FreeThreeService** - Detects double-free-three patterns (illegal move validation)

**JsonService** - Handles decision tree serialization for AI debugging

### Data Structures (`include/structs/`)

- `Position`: Board coordinates (x, y)
- `AiPlay`: AI move result with position and evaluation score
- `IllegalMoves`: Enum for illegal move types
- `LineBlockState`: Pattern analysis state for alignment checking

## Development Notes

### Bitmask Operations
The board uses bitmasks for performance. To check/modify a stone at position (x, y):
- Check: `grid[y] & (1 << x)`
- Set: `grid[y] |= (1 << x)`
- Clear: `grid[y] &= ~(1 << x)`

### AI Search Strategy
MinMax generates moves near existing stones (radius 2) to keep search tractable on 19x19 board. Move candidates are prioritized by tactical importance:

**Threat Detection** (OFFENSIF ET DÉFENSIF):
Le move ordering évalue et priorise automatiquement tous les coups tactiques:

*OFFENSIF* (créer des menaces):
- Créer un 5-en-ligne: 10,000,000 points (VICTOIRE IMMÉDIATE!)
- Créer un 4-en-ligne libre: 900,000 points (menace gagnante)
- Créer un 4-en-ligne semi-bloqué: 300,000 points
- Créer un 3-en-ligne libre: 150,000 points (préparation)
- Captures immédiates: 1,000,000+ points

*DÉFENSIF* (bloquer les menaces adverses):
- Bloquer 5-en-ligne adverse: 5,000,000 points (URGENT!)
- Bloquer 4-en-ligne libre adverse: 800,000 points (TRÈS PRIORITAIRE)
- Bloquer 4-en-ligne semi-bloqué: 400,000 points
- Bloquer 3-en-ligne libre adverse: 200,000 points

**Move Candidate Limits** (ULTRA SÉLECTIF pour profondeur maximale):
- 6 moves à la racine
- 5 moves à depth > 3
- 4 moves à depth > 6
- TOUS les coups critiques sont TOUJOURS inclus:
  - Captures
  - Créer un 4 ou 5 (OFFENSIF)
  - Bloquer un 4 ou 5 adverse (DÉFENSIF)

**Priorité Défensive**: L'IA privilégie une défense solide. Avant de limiter les candidats, elle garantit l'inclusion de TOUS les coups qui:
1. Capturent des pierres adverses
2. Bloquent un 4-en-ligne ou 5-en-ligne adverse

**Capture Prioritization**: Root-level bonus (+2,000,000) ensures immediate captures are taken even when deep search suggests other moves.

**Heuristic Evaluation** (FastEvaluator):
- Immediate captures: 600,000 points per pair
- Four-in-a-row (open): 50,000 | (semi-blocked): 20,000
- Three-in-a-row (open): 10,000 | (semi-blocked): 3,000
- Two-in-a-row (open): 800
- Capture threat detection: 5,000
- Capture pairs (in position): 25,000
- Win condition (5-in-a-row or 10 captures): 1,000,000

**Depth Penalty**: 150 points per move-depth from root (except wins) to prefer immediate tactical gains.

**Performance**:
- **Profondeur atteinte**: 6-10 (early game: 6, mid game: 10 avec détection de victoire)
- **Temps de réflexion**: 500-900ms
- **Philosophie**: FORCE MAXIMALE > vitesse. L'IA doit être IMBATTABLE.
- Branching factor ultra-réduit (6→5→4) pour chercher PROFOND et voir les tactiques complexes

### Testing Framework
Uses Catch2 for unit tests. Tests cover:
- Board operations and state management
- MinMax algorithm correctness
- Win/capture detection
- Legal move validation (including double-free-three)
- Alignment checking in all directions

## Dependencies

- **SFML 3.0.1**: Graphics and window management (audio/network disabled)
- **nlohmann/json 3.11.3**: JSON serialization for decision trees
- **Catch2 3.5.4**: Testing framework

All dependencies fetched automatically via CMake FetchContent during build.

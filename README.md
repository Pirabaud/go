# Gomoku

Gomoku is a two-player strategy game played on a 19x19 grid where players alternate placing stones, aiming to create an unbroken line of five or more stones or capture opponent stones.

---
## Rules
### Basic Rules
- Played on 19x19 board (Goban)
- Black moves first, then players alternate turns
- 2 win conditions:
    - Align 5+ stones in a row (horizontal/vertical/diagonal)
    - Capture 10 opponent stones (5 pairs)

### Capture Rules
- Stones are captured by flanking:
    - Must be exactly 2 opponent stones between your stones
    - Example valid captures:
        * X O O X   → X _ _ X (captures the two O stones)
        * X O O _ X → Not a capture (spaces between X stones)
        * X O O O X → Not a capture (3 stones between)
- Only complete pairs (2 stones) can be captured at once
- Captured pairs are removed from board
- First to capture 10 opponent stones (5 pairs) wins


### Special Conditions
1. Five-stone line can be broken if opponent can capture part of it
2. If player has lost 8 stones (4 pairs), next capture loses the game
3. Game ends when five-stone line cannot be broken by capture

### Move Restrictions
- Double-Free-Three Ban:
    - Cannot play a move that creates two separate open three-in-a-rows
    - Prevents guaranteed wins through forced sequences

### Notes
- Standard Gomoku favors first player; these rules add balance
- No limits on stone count or board positions
- Captures take priority over alignments in endgame

---
## Requirements

- `Make`
- `Java 17`
- `Maven`
- `Git`

## Installation

1. Clone the repository :
```bash
git clone https://github.com/pirabaud/Gomoku.git
```
2. Navigate to the project directory :
```bash
cd Gomoku
```
3. Build the project using Makefile :
```bash
make
```
This will download the required dependencies and build the project.

4. Run the project :
```bash
make run
```
5. Happy playing!******
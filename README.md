# Gomoku

Gomoku is a two-player strategy game played on a 19x19 grid where players alternate placing stones, aiming to create an unbroken line of five or more stones or capture opponent stones.

---
## Rules
### Basic Rules
- Played on 19x19 board (Goban)
- Black moves first, then players alternate turns
- 2 win conditions:
    - Align 5+ stones in a row (horizontal/vertical/diagonal - if the row is not breakable by capture)
    - Capture 10 or more opponent stones (5 pairs)
    - Capture 8 opponent stones and have the possibility to capture at least an other pair

### Capture Rules
- Stones are captured by flanking:
    - Must be exactly 2 opponent stones between your stones
    - Example valid captures:
        * X O O X   → X _ _ X (captures the two O stones)
        * X O O _ X → Not a capture (spaces between X stones)
        * X O O O X → Not a capture (3 stones between)
- Only complete pairs (2 stones) can be captured at once
- Captured pairs are removed from board


### Special Conditions
1. Five-stone line can be broken if opponent can capture part of it
2. If player has lost 8 stones (4 pairs), next capture loses the game
3. Game ends when five-stone line cannot be broken by capture

### Move Restrictions
- Double-Free-Three Ban:
    - A double free-three is taken into account only if the move does not create a capture
    - Cannot play a move that creates two separate open three-in-a-rows
    - Prevents guaranteed wins through forced sequences

### Notes
- Standard Gomoku favors first player; these rules add balance
- No limits on stone count or board positions
- Captures take priority over alignments in endgame


### Bonuses
We make 5 bonuses :
- Sound to the overhaul game
- Pro Opening variant
- History of moves inside a game (LEFT / RIGHT arrow keys to browse history)
- Double stones variant (fun to play)
- 15x15 Goban

### Run the project :
```bash
make run
```

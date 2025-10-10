
#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <vector>

#include "Board.h"


class GameState {
public:
    GameState(GameState* _parent, Board& board, Position moveToMake, bool newMaximizing = false);
    ~GameState() = default;

    Position getBestMove();
    GameState* play(Position pos);

    void setDepth(int newDepth);

    [[nodiscard]] Position getMove() const;
    [[nodiscard]] Board& getBoard();
    [[nodiscard]] bool isBlack() const;
    [[nodiscard]] bool isMaximizing() const;

    bool isTerminal() const;

    std::vector<Position> getPossibleMoves() const;
private:
    Position move;
    int depth = 0;
    int heuristic = 0;
    bool maximizing = true;
    GameState* parent = nullptr;
    std::vector<GameState*> children;
    Board& board;
};


#endif //GAME_STATE_H
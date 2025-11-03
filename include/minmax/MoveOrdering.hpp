#ifndef MOVE_ORDERING_HPP
#define MOVE_ORDERING_HPP

#include <vector>
#include "Board.h"
#include "Position.hpp"

constexpr int MAX_DEPTH = 20;

class MoveOrdering {
public:
    MoveOrdering();

    void clearHistory();
    void addKillerMove(int depth, Position move);
    void updateHistory(Position move, int depth);

    void orderMoves(std::vector<Position>& moves, int depth, Position hashMove, const Board& board, bool isWhite);

private:
    int history[Board::SIZE][Board::SIZE];
    Position killerMoves[MAX_DEPTH][2];

    int getMoveScore(Position move, int depth, Position hashMove, const Board& board, bool isWhite) const;
    bool isThreatMove(Position move, const Board& board, bool isWhite) const;
};

#endif

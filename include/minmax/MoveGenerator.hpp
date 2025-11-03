#ifndef MOVE_GENERATOR_HPP
#define MOVE_GENERATOR_HPP

#include <vector>
#include "Board.h"
#include "Position.hpp"

class MoveGenerator {
public:
    static std::vector<Position> generateMoves(const Board& board, int searchRadius = 2);

private:
    static bool isNearStone(const Board& board, Position pos, int radius);
};

#endif

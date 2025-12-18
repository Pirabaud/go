//
// Created by pirabaud on 12/12/25.
//

#ifndef GOMOKU_DIRECTION_HPP
#define GOMOKU_DIRECTION_HPP
#include "Board.h"

enum Direction {
    HORIZONTAL = 1,
    VERTICAL = Board::SIZE + 1,
    DIAGONAL_TOP_RIGHT = VERTICAL + 1,
    DIAGONAL_TOP_LEFT = VERTICAL - 1,
};

#endif //GOMOKU_DIRECTION_HPP
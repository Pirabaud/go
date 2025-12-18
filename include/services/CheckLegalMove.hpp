#ifndef CHECK_MOVE_SERVICE_H
#define CHECK_MOVE_SERVICE_H

#include "Board.h"
#include "IllegalMoves.hpp"
#include "Position.hpp"
#include "AlignmentChecker.hpp"


class CheckLegalMove {
public:
    static IllegalMoves::Type isLegalMove(
        Position pos, Board &board, const bool &isBlack);
};


#endif //CHECK_MOVE_SERVICE_H

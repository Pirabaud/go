//
// Created by pirabaud on 9/24/25.
//

#ifndef GOMOKU_FREETHREESERVICE_H
#define GOMOKU_FREETHREESERVICE_H

#include "Board.h"


class FreeThreeService {

public:
    static bool isFreeThree(Board &board, Position pos);

private:
    static bool checkDirectionFreeThree(Board::StoneMask &grid, Board::StoneMask &gridOpposite, Position pos, Position dir);

};


#endif //GOMOKU_FREETHREESERVICE_H
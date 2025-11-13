//
// Created by pirabaud on 9/24/25.
//

#ifndef GOMOKU_FREETHREESERVICE_H
#define GOMOKU_FREETHREESERVICE_H

#include "Board.h"


class FreeThreeService {

public:
    static bool isFreeThree(Board &board, Position pos);
    static bool checkDirectionFreeThree(Board &board, Position pos, Position dir);

private:
    static bool checkNotFreeLine(const Board &board, Position pos, Position dir);

};


#endif //GOMOKU_FREETHREESERVICE_H
//
// Created by pirabaud on 6/20/25.
//

#include "../../include/Service/CheckMoveService.h"

#include <iostream>
#include <ostream>

bool CheckMoveService::isLegalMove(const int &x, const int &y, const std::array<uint32_t, Board::SIZE> &gridBack,
                                   const std::array<uint32_t, Board::SIZE> &gridWhite, const bool &isBlack) {
    if (NotInBoard(x, y)) {
        return false;
    }
    if (alreadyStone(x, y, gridBack, gridWhite)) {
        return false;
    }
    if (checkLineCapture(x, y, gridBack, gridWhite, isBlack)) {
        return false;
    }
    return true;
}

bool CheckMoveService::NotInBoard(const int &x, const int &y) {
    if (x < 0 || x >= Board::SIZE || y < 0 || y >= Board::SIZE) {
        return true;
    }
    return false;
}

bool CheckMoveService::alreadyStone(const int &x, const int &y, const std::array<uint32_t, Board::SIZE> &gridBlack,
    const std::array<uint32_t, Board::SIZE> &gridWhite) {
    uint32_t checkStone = 1u << Board::SIZE - 1 - y;
    if (checkStone == gridBlack.at(x) || checkStone == gridWhite.at(x)) {
        return true;
    }
    return false;
}

bool CheckMoveService::checkSelfCapture(const int &dx, const int &dy, const std::array<uint32_t, Board::SIZE> & gridWhite,
    const std::array<uint32_t, Board::SIZE> &gridBlack, uint32_t &stone, const bool &isBlack) {

    if (isBlack) {
        //uint32_t line = gridBlack.at(x + );
        //uint32_t adjacentStone = 1u << Board::SIZE - 1 - y ;

    }

    return false;
}

bool CheckMoveService::canSelfCapture(const int &x, const int &y, const std::array<uint32_t, Board::SIZE> &,
                                      const std::array<uint32_t, Board::SIZE> &, const bool &isBlack) {
    uint32_t checkValue = 1u << Board::SIZE - 1 - y;

    return false;

}

bool CheckMoveService::checkLineCapture(const int &x, const int &y, const std::array<uint32_t, Board::SIZE> &gridWhite,
    const std::array<uint32_t, Board::SIZE> &gridBlack, const bool &isBlack) {
    uint32_t checkLineColor;
    uint32_t checkLineOppositeColor;
    uint32_t stone = 1u << Board::SIZE - 1 - y;
    if (isBlack) {
        checkLineColor = gridBlack.at(x);
        checkLineOppositeColor = gridWhite.at(x);
    }
    else {
        checkLineColor = gridWhite.at(x);
        checkLineOppositeColor = gridBlack.at(x);
    }
    uint32_t checkAdjacentRight = checkLineColor >> Board::SIZE - y - 2 & stone >> Board::SIZE - y - 1;
    if (checkAdjacentRight % 2 != 0) {
        uint32_t checkOppositeColor = checkLineOppositeColor >> Board::SIZE - y - 3 & checkAdjacentRight;
        if (checkOppositeColor % 2 != 0) {
            return true;
        }
        return false;
    }
    return false;
}

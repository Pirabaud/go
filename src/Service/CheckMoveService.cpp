//
// Created by pirabaud on 6/20/25.
//

#include "../../include/Service/CheckMoveService.h"

#include <bitset>
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
    if (checkLineCapture(x, y, gridBack, gridWhite, TODO, TODO)) {
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

bool CheckMoveService::checkDirectionCapture(const int &x, const int &y, const std::array<uint32_t, Board::SIZE> &gridColor,
    const std::array<uint32_t, Board::SIZE> &gridOpposite) {

    std::array<std::pair<int, int>, 4> directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto &[dx, dy] : directions) {
        if (checkCapture(x, y, gridColor, gridOpposite, dx, dy)) {
            return true;
        }
        if (checkCapture(x, y, gridColor, gridOpposite, -dx, -dy)) {
            return true;
        }
    }
    return false;
}

inline bool checkBitSet(const uint32_t &val, const int &bit) {
    return bit >= 0 && bit < Board::SIZE && val & (1u << bit);
}

bool CheckMoveService::checkCapture(const int &x, const int &y, const std::array<uint32_t, Board::SIZE> &gridColor,
    const std::array<uint32_t, Board::SIZE> &gridOpposite, const int &dx, const int &dy) {

    const uint32_t lineColorAdjacent = gridColor.at(x + dx);
    const uint32_t lineOppositeAdjacent1 = gridOpposite.at(x + dx * 2);
    const uint32_t lineOppositeAdjacent2 = gridOpposite.at(x - dx);

    const int maskColorAdjacent = Board::SIZE - y - dy;
    const int maskOppositeAdjacent1 = Board::SIZE - y - dy * 2;
    const int maskOppositeAdjacent2 = Board::SIZE - y - dy * -2;


    if (checkBitSet(lineColorAdjacent, maskColorAdjacent)
        && checkBitSet(lineOppositeAdjacent1,  maskOppositeAdjacent1)
        && checkBitSet(lineOppositeAdjacent2, maskOppositeAdjacent2)) {
        return true;
    }
    return false;
}


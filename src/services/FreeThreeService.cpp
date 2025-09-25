//
// Created by pirabaud on 9/24/25.
//

#include "../../include/services/FreeThreeService.h"

#include <algorithm>
#include <iostream>

#include "AlignmentChecker.hpp"
#include "catch2/internal/catch_clara.hpp"

bool FreeThreeService::isFreeThree(Board &board, Position pos) {

    Board::StoneMask grid = board.isBlackStoneAt(pos) ? board.getGridBlack() : board.getGridWhite();
    Board::StoneMask gridOpposite = board.isBlackStoneAt(pos) ? board.getGridWhite() : board.getGridBlack();

    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };
    board.printBoard();
    for (auto &[dx, dy] : directions) {
        if (checkDirectionFreeThree(grid, gridOpposite, pos, {dx, dy})) {
            return true;
        }
        if (checkDirectionFreeThree(grid, gridOpposite, pos, {-dx, -dy})) {
            return true;
        }
    }

    return false;
}

bool FreeThreeService::checkDirectionFreeThree(Board::StoneMask &grid, Board::StoneMask &gridOpposite, Position pos, Position dir) {

    int hole = 0;
    int count = 1;
    int cx = pos.x  + dir.x;
    int cy = pos.y + dir.y;
    for (int i = 0; i < 2; i++) {
        if ( cx < 0 || cx >= Board::SIZE || cy < 0 || cy >= Board::SIZE) {
            break;
        }
        if (((grid.at(cx) >> (Board::SIZE - 1 - cy)) & 1) == 0) {
            hole++;
        }

        if (hole == 2) {
            break;
        }
        count++;
        cx += dir.x;
        cy += dir.y;
    }
    if (cx < 0 || cx >= Board::SIZE || cy < 0 || cy >= Board::SIZE) {
        return false;
    }
    if (((gridOpposite.at(cx) >> (Board::SIZE - 1 - cy)) & 1) == 1) {
        return false;
    }
    if (count == 3 && pos.x - dir.x >= 0 && pos.y - dir.y >= 0 && pos.x + dir.x < Board::SIZE && pos.y + dir.y < Board::SIZE) {
        return true;
    }
    return false;
}



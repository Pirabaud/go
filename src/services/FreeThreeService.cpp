//
// Created by pirabaud on 9/24/25.
//

#include "../../include/services/FreeThreeService.h"

#include <algorithm>
#include <iostream>

#include "AlignmentChecker.hpp"
#include "CheckLegalMove.hpp"

bool FreeThreeService::isFreeThree(Board &board, const Position pos) {

    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };
    board.printBoard();
    for (auto &[dx, dy] : directions) {
        if (checkDirectionFreeThree(board, pos, {dx, dy})) {
            return true;
        }
        if (checkDirectionFreeThree(board, pos, {-dx, -dy})) {
            return true;
        }
    }

    return false;
}

bool FreeThreeService::checkDirectionFreeThree(Board &board, const Position pos, const Position dir) {
    int hole = 0;
    int count = 1;
    int cx = pos.x  + dir.x;
    int cy = pos.y + dir.y;

    //check free before count alignment
    if (checkNotFreeLine(board, pos, {-dir.x, -dir.y})) {
        return false;
    }

    //check alignment
    for (int i = 0; i < 2; i++) {

        //check board
        if ( cx < 0 || cx >= Board::SIZE || cy < 0 || cy >= Board::SIZE) {
            break;
        }

        //check stone opp
        bool oppStone = board.isBlackStoneAt(pos) ? board.isWhiteStoneAt({cx, cy}) : board.isBlackStoneAt({cx, cy});

        board.printBoard();
        if (oppStone) {
            //std::cout << pos.x << " " << pos.y << std::endl;
            return false;
        }

        //check hole
        if (!board.isBlackStoneAt({cx, cy}) && !board.isWhiteStoneAt({cx, cy})) {
            hole++;
        }
        if (hole == 2) {
            break;
        }

        //board.printBoard();


        count++;
        cx += dir.x;
        cy += dir.y;
    }
    //check free
    if (!checkNotFreeLine(board, {cx, cy}, dir) && count == 3) {
        return true;
    }
    return false;
}

bool FreeThreeService::checkNotFreeLine(const Board &board, const Position pos, const Position dir) {
    if (CheckLegalMove::notInBoard({pos.x + dir.x, pos.y + dir.y})) {
        return true;
    }

    if (board.isBlackStoneAt(pos) ? board.isWhiteStoneAt({pos.x + dir.x, pos.y + dir.y}) : board.isBlackStoneAt({pos.x + dir.x, pos.y + dir.y})) {
        return true;
    }

    return false;
}





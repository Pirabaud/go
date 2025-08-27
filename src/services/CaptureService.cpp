//
// Created by afaby on 6/27/25.
//

#include "CaptureService.hpp"

#include <iostream>

#include "Position.hpp"
#include "SFML/Graphics/Color.hpp"

bool CaptureService::resolveCaptureAtPosition(Board & board, const Position pos, const bool color) {
    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto& [x, y] : directions) {
        if (resolveCaptureAtPositionInDirection(board, pos, Position(x, y), color)) {
            return true;
        }
        if (resolveCaptureAtPositionInDirection(board, pos, Position(-x, -y), color)) {
            return true;
        }
    }
    return false;
}

bool CaptureService::resolveCaptureAtPositionInDirection(Board &board, const Position pos, const Position dir, const bool color) {
    // Check overflow
    if (pos.x + dir.x > Board::SIZE - 1 || pos.x + dir.x < 0 ||
        pos.x + dir.x * 2 > Board::SIZE - 1 || pos.x + dir.x * 2 < 0 ||
        pos.x + dir.x * 3 > Board::SIZE - 1 || pos.x + dir.x * 3 < 0 ||
        pos.y + dir.y > Board::SIZE - 1 || pos.y + dir.y < 0 ||
        pos.y + dir.y * 2 > Board::SIZE - 1 || pos.y + dir.y * 2 < 0 ||
        pos.y + dir.y * 3 > Board::SIZE - 1 || pos.y + dir.y * 3 < 0) {
        return false;
    }

    const Board::StoneMask allyMask = color ? board.getGridWhite() : board.getGridBlack();
    const Board::StoneMask& enemyMask = color ? board.getGridBlack() : board.getGridWhite();

    const bool ex1 = Board::isStoneAt(enemyMask, Position(pos.x + dir.x,pos.y + dir.y));
    const bool ex2 = Board::isStoneAt(enemyMask, Position(pos.x + dir.x * 2, pos.y + dir.y * 2));
    const bool ex3 = Board::isStoneAt(allyMask, Position(pos.x + dir.x * 3, pos.y + dir.y * 3));
    return ex1 && ex2 && ex3;
}

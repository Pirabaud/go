//
// Created by afaby on 6/27/25.
//

#include "CaptureService.hpp"
#include "Position.hpp"

bool CaptureService::resolveCaptureAtPosition(Board & board, const Position pos, const bool isBlack) {
    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto& [x, y] : directions) {
        if (resolveCaptureAtPositionInDirection(board, pos, Position{x, y}, isBlack)) {
            return true;
        }
         if (resolveCaptureAtPositionInDirection(board, pos, Position{-x, -y}, isBlack)) {
            return true;
        }
    }
    return false;
}

bool CaptureService::resolveCaptureAtPositionInDirection(Board &board, const Position pos, const Position dir, const bool isBlack) {
    // Check overflow
    if (pos.x + dir.x > Board::SIZE - 1 || pos.x + dir.x < 0 ||
        pos.x + dir.x * 2 > Board::SIZE - 1 || pos.x + dir.x * 2 < 0 ||
        pos.x + dir.x * -1 > Board::SIZE - 1 || pos.x + dir.x * -1 < 0 ||
        pos.y + dir.y > Board::SIZE - 1 || pos.y + dir.y < 0 ||
        pos.y + dir.y * 2 > Board::SIZE - 1 || pos.y + dir.y * 2 < 0 ||
        pos.y + dir.y * -1 > Board::SIZE - 1 || pos.y + dir.y * -1 < 0) {
        return false;
    }

    const Board::StoneMask allyMask = isBlack ? board.getGridBlack() : board.getGridWhite();
    const Board::StoneMask& enemyMask = isBlack ? board.getGridWhite() : board.getGridBlack();

    const bool ex1 = Board::isStoneAt(allyMask, Position{pos.x + dir.x, pos.y + dir.y});
    const bool ex2 = Board::isStoneAt(enemyMask, Position{pos.x + dir.x * 2, pos.y + dir.y * 2});
    const bool ex3 = Board::isStoneAt(enemyMask, Position{pos.x - dir.x, pos.y - dir.y});
    return ex1 && ex2 && ex3;
}

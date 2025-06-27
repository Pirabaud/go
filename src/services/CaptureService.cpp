//
// Created by afaby on 6/27/25.
//

#include "CaptureService.hpp"

#include <iostream>

void CaptureService::resolveCaptures(Board& board) {
    for (int row = 0; row < Board::SIZE; row++) {
        for (int col = 0; col < Board::SIZE; col++) {
            resolveCaptureAtPosition(board, row, col);
        }
    }
}

void CaptureService::resolveCaptureAtPosition(Board& board, const int& row, const int& col) {
    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto& [dx, dy] : directions) {
        resolveCaptureAtPositionInDirection(board, row, col, dx, dy);
    }
}

void CaptureService::resolveCaptureAtPositionInDirection(Board& board, const int& row, const int& col, const int& dx,
                                                         const int& dy) {
    // Check overflow
    if (row + dx > Board::SIZE - 1 || row + dx < 0 ||
        row + dx * 2 > Board::SIZE - 1 || row + dx * 2 < 0 ||
        row + dx * 3 > Board::SIZE - 1 || row + dx * 3 < 0 ||
        col + dy > Board::SIZE - 1 || col + dy < 0 ||
        col + dy * 2 > Board::SIZE - 1 || col + dy * 2 < 0 ||
        col + dy * 3 > Board::SIZE - 1 || col + dy * 3 < 0) {
        return;
    }

    const bool isWhiteStone = board.isWhiteStoneAt(row, col);
    const bool isBlackStone = board.isBlackStoneAt(row, col);
    if (!isWhiteStone && !isBlackStone) {
        return;
    }
    const Board::StoneMask allyMask = isWhiteStone ? board.getGridWhite() : board.getGridBlack();
    Board::StoneMask& enemyMask = isWhiteStone ? board.getGridBlack() : board.getGridWhite();

    const bool ex1 = Board::isStoneAt(enemyMask, row + dx, col + dy);
    const bool ex2 = Board::isStoneAt(enemyMask, row + dx * 2, col + dy * 2);
    const bool ex3 = Board::isStoneAt(allyMask, row + dx * 3, col + dy * 3);
    const bool isThereACapture = ex1 && ex2 && ex3;
    if (!isThereACapture) return;
    Board::removeStoneAt(enemyMask, row + dx, col + dy);
    Board::removeStoneAt(enemyMask, row + dx * 2, col + dy * 2);
}




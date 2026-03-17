#include "CheckWinService.hpp"
#include <algorithm>
#include "AlignmentChecker.hpp"


const sf::Color* CheckWinService::isWin(Board& board) {
    if (board.getBlackCaptured() >= 10) {
        return &sf::Color::White;
    }
    if (board.getWhiteCaptured() >= 10) {
        return &sf::Color::Black;
    }

    if (AlignmentChecker::checkWinAlignment(board, false, 1) ||
        AlignmentChecker::checkWinAlignment(board, false, Board::SIZE + 1) ||
        AlignmentChecker::checkWinAlignment(board, false, Board::SIZE) ||
        AlignmentChecker::checkWinAlignment(board, false, Board::SIZE + 2)) {
        return &sf::Color::White;
    }
    if (AlignmentChecker::checkWinAlignment(board, true, 1) ||
        AlignmentChecker::checkWinAlignment(board, true, Board::SIZE + 1) ||
        AlignmentChecker::checkWinAlignment(board, true, Board::SIZE) ||
        AlignmentChecker::checkWinAlignment(board, true, Board::SIZE + 2)) {
        return &sf::Color::Black;
    }

    return nullptr;
}

std::array<int, 15> CheckWinService::getWinBlockingIndices(Board& board, const bool isBlack) {
    std::array<int, 15> result = {};
    std::ranges::fill(result, -1);
    int nextIndex = 0;
    const auto allyBitBoard = isBlack ? board.getBitBoardBlack() : board.getBitBoardWhite();
    const auto enemyBitBoard = isBlack ? board.getBitBoardWhite() : board.getBitBoardBlack();
    const auto blockingIndexHorizontal = AlignmentChecker::checkBreakableAlignment(allyBitBoard, enemyBitBoard, 1);
    const auto blockingIndexVertical = AlignmentChecker::checkBreakableAlignment(
        allyBitBoard, enemyBitBoard, Board::SIZE + 1);
    const auto blockingIndexDiagonalTopLeft = AlignmentChecker::checkBreakableAlignment(
        allyBitBoard, enemyBitBoard, Board::SIZE);
    const auto blockingIndexDiagonalTopRight = AlignmentChecker::checkBreakableAlignment(
        allyBitBoard, enemyBitBoard, Board::SIZE + 2);

    int blockingIndexIndex = 0;
    while (blockingIndexHorizontal[blockingIndexIndex++] != -1) {
        if (nextIndex >= result.size()) {
            return result;
        }
        result[nextIndex++] = blockingIndexHorizontal[blockingIndexIndex - 1];
    }
    blockingIndexIndex = 0;
    while (blockingIndexVertical[blockingIndexIndex++] != -1) {
        if (nextIndex >= result.size()) {
            return result;
        }
        result[nextIndex++] = blockingIndexVertical[blockingIndexIndex - 1];
    }
    blockingIndexIndex = 0;
    while (blockingIndexDiagonalTopLeft[blockingIndexIndex++] != -1) {
        if (nextIndex >= result.size()) {
            return result;
        }
        result[nextIndex++] = blockingIndexDiagonalTopLeft[blockingIndexIndex - 1];
    }
    blockingIndexIndex = 0;
    while (blockingIndexDiagonalTopRight[blockingIndexIndex++] != -1) {
        if (nextIndex >= result.size()) {
            return result;
        }
        result[nextIndex++] = blockingIndexDiagonalTopRight[blockingIndexIndex - 1];
    }
    return result;
}

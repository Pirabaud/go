#include "../../include/services/CheckWinService.hpp"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <ostream>
#include <vector>

#include "AlignmentChecker.hpp"
#include "Direction.hpp"
#include "LineBlockState.hpp"


// TODO optimize
// Check local win -> if 5 in a row found, check if breakable
const sf::Color* CheckWinService::isWin(Board& board)
{
    if (board.getBlackCaptured() >= 10) {
        return &sf::Color::White;
    }
    if (board.getWhiteCaptured() >= 10) {
        return &sf::Color::Black;
    }

    if (AlignmentChecker::checkWinAlignment(board, false, Direction::HORIZONTAL) ||
        AlignmentChecker::checkWinAlignment(board, false, Direction::VERTICAL) ||
        AlignmentChecker::checkWinAlignment(board, false, Direction::DIAGONAL_TOP_LEFT) ||
        AlignmentChecker::checkWinAlignment(board, false, Direction::DIAGONAL_TOP_RIGHT)) {
        return &sf::Color::White;
        }
    if (AlignmentChecker::checkWinAlignment(board, true, Direction::HORIZONTAL) ||
        AlignmentChecker::checkWinAlignment(board, true, Direction::VERTICAL) ||
        AlignmentChecker::checkWinAlignment(board, true, Direction::DIAGONAL_TOP_LEFT) ||
        AlignmentChecker::checkWinAlignment(board, true, Direction::DIAGONAL_TOP_RIGHT)) {
        return &sf::Color::Black;
        }

    return nullptr;
}

std::array<int, 15> CheckWinService::getWinBlockingIndices(Board& board, const bool isBlack)
{
    std::array<int, 15> result = {};
    std::ranges::fill(result, -1);
    int nextIndex = 0;
    const auto allyBitBoard = isBlack ? board.getBitBoardBlack() : board.getBitBoardWhite();
    const auto enemyBitBoard = isBlack ? board.getBitBoardWhite() : board.getBitBoardBlack();
    const auto blockingIndexHorizontal = AlignmentChecker::checkBreakableAlignment(allyBitBoard, enemyBitBoard, Direction::HORIZONTAL);
    const auto blockingIndexVertical = AlignmentChecker::checkBreakableAlignment(allyBitBoard, enemyBitBoard, Direction::VERTICAL);
    const auto blockingIndexDiagonalTopLeft = AlignmentChecker::checkBreakableAlignment(allyBitBoard, enemyBitBoard, Direction::DIAGONAL_TOP_LEFT);
    const auto blockingIndexDiagonalTopRight = AlignmentChecker::checkBreakableAlignment(allyBitBoard, enemyBitBoard, Direction::DIAGONAL_TOP_RIGHT);

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

bool CheckWinService::isBreakableWinAlignment(Board &allyBoard, Board &enemyBoard, int startIndex, Direction dir) {
    return false;
}


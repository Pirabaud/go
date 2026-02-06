//
// Created by afaby on 6/27/25.
//

#include "CaptureService.hpp"

#include <iostream>
#include <ostream>

#include "Direction.hpp"
#include "Position.hpp"

int CaptureService::checkCapture(Board& board,  const Position pos, const bool isBlack) {
    int result = 0;
    const int global_index = pos.x * (Board::SIZE + 1) + pos.y;

    constexpr std::array<int, 4> directions = {
        HORIZONTAL,
        VERTICAL,
        DIAGONAL_TOP_RIGHT,
        DIAGONAL_TOP_LEFT,
    };

    for (const int dir: directions) {
        result += checkCaptureInDirection(board, global_index, dir, isBlack);
        result += checkCaptureInDirection(board, global_index, -dir, isBlack);
    }
    return result;
}

bool CaptureService::winLineBreakable(const std::array<uint64_t, 6> &allyBitBoard,
    const std::array<uint64_t, 6> &enemyBitBoard, const int startIndex, const int dirAlignment) {

    constexpr std::array<int, 4> directions = {
        HORIZONTAL,
        VERTICAL,
        DIAGONAL_TOP_RIGHT,
        DIAGONAL_TOP_LEFT,
    };
        for (int i = 0; i < 5; i++) {
            const int checkStone = startIndex + i * dirAlignment;
            for (const int dir: directions) {
                if (dirAlignment == dir || -dirAlignment == dir) {
                    continue;
                }
                if (Board::isBitAt(allyBitBoard, checkStone + dir)) {
                    const bool isEnemyAfter = Board::isBitAt(enemyBitBoard, checkStone + 2 * dir);
                    const bool isEnemyBefore = Board::isBitAt(enemyBitBoard, checkStone -  dir);

                    const bool isEmptyAfter = !isEnemyAfter && !Board::isBitAt(allyBitBoard, checkStone + 2 * dir);
                    const bool isEmptyBefore = !isEnemyBefore && !Board::isBitAt(allyBitBoard,checkStone - dir);

                    if ((isEnemyAfter && isEmptyBefore) || (isEnemyBefore && isEmptyAfter)) return true;
                }
                if (Board::isBitAt(allyBitBoard, checkStone - dir)) {
                    const bool isEnemyAfter = Board::isBitAt(enemyBitBoard, checkStone  - 2 * dir);
                    const bool isEnemyBefore = Board::isBitAt(enemyBitBoard, checkStone + dir);

                    const bool isEmptyAfter = !isEnemyAfter && !Board::isBitAt(allyBitBoard, checkStone - 2 * dir);
                    const bool isEmptyBefore = !isEnemyBefore && !Board::isBitAt(allyBitBoard, checkStone + dir);

                    if ((isEnemyAfter && isEmptyBefore) || (isEnemyBefore && isEmptyAfter)) return true;
                }
            }
    }
    return false;
}

int CaptureService::checkCaptureInDirection(Board& board, const int globalIndex,
                                            const int dir, const bool isBlack) {
    const std::array<uint64_t, 6>& allyBitBoard = isBlack ? board.getBitBoardBlack() : board.getBitBoardWhite();
    std::array<uint64_t, 6>& enemyBitBoard = isBlack ? board.getBitBoardWhite() : board.getBitBoardBlack();

    //check ally
    if (!Board::isBitAt(allyBitBoard, globalIndex + 3 * dir)) {
        return 0;
    }
    const int firstEnemyGlobalIndex = (globalIndex + 1 * dir);
    const int secondEnemyGlobalIndex = (globalIndex + 2 * dir);
    const int firstEnemyArrayIndex = firstEnemyGlobalIndex / 64;
    const int firstEnemyIndex = firstEnemyGlobalIndex % 64;
    const int secondEnemyArrayIndex = secondEnemyGlobalIndex / 64;
    const int secondEnemyIndex = secondEnemyGlobalIndex % 64;
    const uint64_t firstEnemyMask = 1ULL << firstEnemyIndex;
    const uint64_t secondEnemyMask = 1ULL << secondEnemyIndex;


    if (enemyBitBoard[firstEnemyArrayIndex] & firstEnemyMask && enemyBitBoard[secondEnemyArrayIndex] &
        secondEnemyMask) {
        isBlack ? board.removeWhiteStone(firstEnemyGlobalIndex) : board.removeBlackStone(firstEnemyGlobalIndex);
        isBlack ? board.removeWhiteStone(secondEnemyGlobalIndex) : board.removeBlackStone(secondEnemyGlobalIndex);
        isBlack ? board.addCaptures(true, 2) : board.addCaptures(false, 2);
        return 2;
    }
    return 0;
}

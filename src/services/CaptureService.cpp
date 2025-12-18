//
// Created by afaby on 6/27/25.
//

#include "CaptureService.hpp"

#include <iostream>
#include <ostream>

#include "Direction.hpp"
#include "Position.hpp"

void CaptureService::checkCapture(const std::array<uint64_t, 6> &allyBitBoard, std::array<uint64_t, 6> &enemyBitBoard,
                                  const Position pos) {
    const int global_index = pos.x * (Board::SIZE + 1) + pos.y;

    constexpr std::array<int, 4> directions = {
        HORIZONTAL,
        VERTICAL,
        DIAGONAL_TOP_RIGHT,
        DIAGONAL_TOP_LEFT,
    };

    for (const int dir : directions) {
        checkCaptureInDirection(allyBitBoard, enemyBitBoard, global_index, dir);
        checkCaptureInDirection(allyBitBoard, enemyBitBoard, global_index, -dir);
    }
}

void CaptureService::checkCaptureInDirection(const std::array<uint64_t, 6> &allyBitBoard,
                                              std::array<uint64_t, 6> &enemyBitBoard, const int globalIndex, const int dir) {
    //check ally
    if (!Board::isBitAt(allyBitBoard, globalIndex + 3 * dir)) {
        return;
    }

    const int firstEnemyArrayIndex = (globalIndex + 1 * dir) / 64;
    const int firstEnemyIndex = (globalIndex + 1 * dir) % 64;
    const int secondEnemyArrayIndex = (globalIndex + 2 * dir) / 64;
    const int secondEnemyIndex = (globalIndex + 2 * dir) % 64;
    const uint64_t firstEnemyMask = 1ULL << firstEnemyIndex;
    const uint64_t secondEnemyMask = 1ULL << secondEnemyIndex;


    if ( enemyBitBoard[firstEnemyArrayIndex] & firstEnemyMask && enemyBitBoard[secondEnemyArrayIndex] & secondEnemyMask) {
        enemyBitBoard[firstEnemyArrayIndex] &= ~firstEnemyMask;
        enemyBitBoard[secondEnemyArrayIndex] &= ~secondEnemyMask;
        std::cout << "capture" << std::endl;
    }
}

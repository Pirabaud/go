//
// Created by afaby on 6/27/25.
//

#include "CaptureService.hpp"

#include <iostream>
#include <ostream>

#include "Direction.hpp"
#include "Position.hpp"

bool CaptureService::checkCapture(const std::array<uint64_t, 6>& allyBitBoard,  std::array<uint64_t, 6>& enemyBitBoard, Position pos) {
    std::cout << pos.x << " " << pos.y << std::endl;
    const int globalIndex = pos.x * (Board::SIZE + 1) + pos.y;
    const bool captureHorizontalRight = checkCaptureInDirection(allyBitBoard, enemyBitBoard, globalIndex, HORIZONTAL_RIGHT);
    const bool captureHorizontalLeft = checkCaptureInDirection(allyBitBoard, enemyBitBoard, globalIndex, HORIZONTAL_LEFT);
    const bool captureVerticalTop = checkCaptureInDirection(allyBitBoard, enemyBitBoard, globalIndex, VERTICAL_TOP);
    const bool captureVerticalDown = checkCaptureInDirection(allyBitBoard, enemyBitBoard, globalIndex, VERTICAL_DOWN);
    const bool captureDiagonalTopLeft = checkCaptureInDirection(allyBitBoard, enemyBitBoard, globalIndex, DIAGONAL_TOP_LEFT);
    const bool captureDiagonalTopRight = checkCaptureInDirection(allyBitBoard, enemyBitBoard, globalIndex, DIAGONAL_TOP_RIGHT);
    const bool captureDiagonalBottomLeft = checkCaptureInDirection(allyBitBoard, enemyBitBoard, globalIndex, DIAGONAL_BOTTOM_LEFT);
    const bool captureDiagonalBottomRight = checkCaptureInDirection(allyBitBoard, enemyBitBoard, globalIndex, DIAGONAL_BOTTOM_RIGHT);

    //std::cout << captureHorizontalLeft << captureHorizontalRight << std::endl;

    return captureHorizontalRight || captureHorizontalLeft || captureVerticalTop ||
            captureVerticalDown || captureDiagonalTopLeft || captureDiagonalTopRight ||
            captureDiagonalBottomLeft || captureDiagonalBottomRight;

}

bool CaptureService::checkCaptureInDirection(const std::array<uint64_t, 6> &allyBitBoard,
                                              std::array<uint64_t, 6> &enemyBitBoard, int globalIndex, int dir) {
    const bool isFirstAlly = Board::isBitAt(allyBitBoard, globalIndex + (0 * dir));
    if (dir == -1) {
        std::cout << isFirstAlly << std::endl;
    }
    const bool isFirstEnemy = Board::isBitAt(enemyBitBoard, globalIndex + (1 * dir));
    if (dir == -1) {
        std::cout << isFirstEnemy << std::endl;
    }
    const bool isSecondEnemy = Board::isBitAt(enemyBitBoard, globalIndex + (2 * dir));
    if (dir == -1) {
        std::cout << isSecondEnemy << std::endl;
    }
    const bool isSecondAlly = Board::isBitAt(allyBitBoard, globalIndex + (3 * dir));
    if (dir == -1) {
        std::cout << isSecondAlly << std::endl;
    }

    if (isFirstAlly && isFirstEnemy && isSecondEnemy && isSecondAlly) {
        Board::clearBitAt(enemyBitBoard, globalIndex + (1 * dir));
        Board::clearBitAt(enemyBitBoard, globalIndex + (2 * dir));
        return true;
    }
    return false;
}

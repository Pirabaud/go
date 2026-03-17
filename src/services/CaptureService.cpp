#include "CaptureService.hpp"
#include <algorithm>

#include "CheckLegalMove.hpp"

int CaptureService::checkCapture(Board &board, int globalIndex, const bool isBlack, int *capture, int &count) {
    int result = 0;


    const std::array<int, 4> directions = {1, Board::SIZE + 1, Board::SIZE, Board::SIZE + 2};
    for (const int dir: directions) {
        result += checkCaptureInDirection(board, globalIndex, dir, isBlack, capture, count);
        result += checkCaptureInDirection(board, globalIndex, -dir, isBlack, capture, count);
    }
    return result;
}

bool CaptureService::winLineBreakable(Board& board, const bool isBlack, const int startIndex, const int dirAlignment) {
    const auto allyBitBoard = isBlack ? board.getBitBoardBlack() : board.getBitBoardWhite();
    const auto enemyBitBoard = isBlack ? board.getBitBoardWhite() : board.getBitBoardBlack();
    const std::array<int, 4> directions = {1, Board::SIZE + 1, Board::SIZE, Board::SIZE + 2};
        for (int i = 0; i < 5; i++) {

            const int checkStone = startIndex + i * dirAlignment;

            for (const int dir: directions) {
                int afterIndex = checkStone + 2 * dir;
                int beforeIndex = checkStone - 2 * dir;

                if (afterIndex < 0 || afterIndex >= (Board::SIZE * (Board::SIZE + 1)) || beforeIndex < 0 || beforeIndex >= (Board::SIZE * (Board::SIZE + 1))) {
                }
                if (dirAlignment == dir || -dirAlignment == dir) {
                    continue;
                }
                if (Board::isBitAt(allyBitBoard, checkStone + dir)) {
                    const bool isEnemyAfter = Board::isBitAt(enemyBitBoard, checkStone + 2 * dir);
                    const bool isEnemyBefore = Board::isBitAt(enemyBitBoard, checkStone -  dir);

                    const bool isEmptyAfter = !isEnemyAfter && !Board::isBitAt(allyBitBoard, checkStone + 2 * dir);
                    const bool isEmptyBefore = !isEnemyBefore && !Board::isBitAt(allyBitBoard,checkStone - dir);

                    if (
                        (isEnemyAfter && isEmptyBefore && CheckLegalMove::isLegalMove(checkStone - dir, board, !isBlack) == IllegalMoves::Type::NONE)
                        || (isEnemyBefore && isEmptyAfter && CheckLegalMove::isLegalMove(checkStone + 2 * dir, board,  !isBlack) == IllegalMoves::Type::NONE)
                        ) {
                        return true;
                    }
                }
                if (Board::isBitAt(allyBitBoard, checkStone - dir)) {
                    const bool isEnemyAfter = Board::isBitAt(enemyBitBoard, checkStone  - 2 * dir);
                    const bool isEnemyBefore = Board::isBitAt(enemyBitBoard, checkStone + dir);

                    const bool isEmptyAfter = !isEnemyAfter && !Board::isBitAt(allyBitBoard, checkStone - 2 * dir);
                    const bool isEmptyBefore = !isEnemyBefore && !Board::isBitAt(allyBitBoard, checkStone + dir);

                    if (
                        (isEnemyAfter && isEmptyBefore && CheckLegalMove::isLegalMove(checkStone + dir, board,  !isBlack) == IllegalMoves::Type::NONE)
                        || (isEnemyBefore && isEmptyAfter && CheckLegalMove::isLegalMove(checkStone - 2 * dir, board,  !isBlack) == IllegalMoves::Type::NONE)
                        ) {
                        return true;
                        }                }
            }
    }
    return false;
}

// Return index where it is possible to capture and break the alignment.
std::array<int, 15> CaptureService::getBlockingCaptureIndex(const std::array<uint64_t, 6>& allyBitBoard,
                                                            const std::array<uint64_t, 6>& enemyBitBoard,
                                                            int startIndex, int dirAlignment) {
    std::array<int, 15> result = {};
    std::ranges::fill(result, -1);
    int nextIndex = 0;

    const std::array<int, 4> directions = {1, Board::SIZE + 1, Board::SIZE, Board::SIZE + 2};
        for (int i = 0; i < 5; i++) {

            const int checkStone = startIndex + i * dirAlignment;

            for (const int dir: directions) {
                int afterIndex = checkStone + 2 * dir;
                int beforeIndex = checkStone - 2 * dir;

                if (afterIndex < 0 || afterIndex >= (Board::SIZE * (Board::SIZE + 1)) || beforeIndex < 0 || beforeIndex >= (Board::SIZE * (Board::SIZE + 1))) {
                    continue;
                }
                if (dirAlignment == dir || -dirAlignment == dir) {
                    continue;
                }
                if (Board::isBitAt(allyBitBoard, checkStone + dir)) {
                    const bool isEnemyAfter = Board::isBitAt(enemyBitBoard, checkStone + 2 * dir);
                    const bool isEnemyBefore = Board::isBitAt(enemyBitBoard, checkStone -  dir);

                    const bool isEmptyAfter = !isEnemyAfter && !Board::isBitAt(allyBitBoard, checkStone + 2 * dir);
                    const bool isEmptyBefore = !isEnemyBefore && !Board::isBitAt(allyBitBoard,checkStone - dir);

                    if (isEnemyAfter && isEmptyBefore) {
                        if (nextIndex >= result.size()) {
                            return result;
                        }
                        result[nextIndex++] = checkStone - dir;
                    }
                    if (isEnemyBefore && isEmptyAfter) {
                        if (nextIndex >= result.size()) {
                            return result;
                        }
                        result[nextIndex++] = checkStone + 2 * dir;
                    }
                }
                if (Board::isBitAt(allyBitBoard, checkStone - dir)) {
                    const bool isEnemyAfter = Board::isBitAt(enemyBitBoard, checkStone  - 2 * dir);
                    const bool isEnemyBefore = Board::isBitAt(enemyBitBoard, checkStone + dir);

                    const bool isEmptyAfter = !isEnemyAfter && !Board::isBitAt(allyBitBoard, checkStone - 2 * dir);
                    const bool isEmptyBefore = !isEnemyBefore && !Board::isBitAt(allyBitBoard, checkStone + dir);

                    if (isEnemyAfter && isEmptyBefore) {
                        if (nextIndex >= result.size()) {
                            return result;
                        }
                        result[nextIndex++] = checkStone + dir;
                    }
                    if (isEnemyBefore && isEmptyAfter) {
                        if (nextIndex >= result.size()) {
                            return result;
                        }
                        result[nextIndex++] = checkStone - 2 * dir;
                    }
                }
            }
    }
    return result;
}

int CaptureService::checkCaptureInDirection(Board& board, const int globalIndex,
                                            const int dir, const bool isBlack, int* capture, int &count) {
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
        capture[count++] = firstEnemyGlobalIndex;
        capture[count++] = secondEnemyGlobalIndex;
        return 2;
    }
    return 0;
}


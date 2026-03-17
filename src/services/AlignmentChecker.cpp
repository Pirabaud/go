#include "AlignmentChecker.hpp"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <bit>
#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"



bool AlignmentChecker::checkWinAlignment(Board& board, const bool isBlack, const int dir) {
    const auto allyBitBoard = isBlack ? board.getBitBoardBlack() : board.getBitBoardWhite();

    const std::array<uint64_t, 6> maskBitBoardPairs = Board::shift_right_board(allyBitBoard, dir);
    const std::array<uint64_t, 6> bitBoardCheckPairs = Board::bitBoardAnd(allyBitBoard,maskBitBoardPairs);

    const std::array<uint64_t, 6> maskBitBoardCheckQuads = Board::shift_right_board(bitBoardCheckPairs, dir * 2);
    const std::array<uint64_t, 6> bitBoardCheckQuads = Board::bitBoardAnd(bitBoardCheckPairs, maskBitBoardCheckQuads);

    const std::array<uint64_t, 6> maskBitBoardFinal = Board::shift_right_board(bitBoardCheckQuads, dir);
    std::array<uint64_t, 6> bitBoardFinal = Board::bitBoardAnd(bitBoardCheckQuads, maskBitBoardFinal);

    for (int i = 0; i < 6; i++) {
        while (bitBoardFinal[i] != 0) {
            if (!CaptureService::winLineBreakable(
                board,
                isBlack,
                i * 64 + std::countr_zero(bitBoardFinal[i]),
                dir)) return true;
            bitBoardFinal[i] &= bitBoardFinal[i] - 1;
        }
    }
    return false;
}


std::array<int, 15> AlignmentChecker::checkBreakableAlignment(const std::array<uint64_t, 6> &allyBitBoard, const std::array<uint64_t, 6> &enemyBitBoard, const int dir) {
    std::array<int, 15> result = {};
    std::ranges::fill(result, -1);
    int nextIndex = 0;
    const std::array<uint64_t, 6> maskBitBoardPairs = Board::shift_right_board(allyBitBoard, dir);
    const std::array<uint64_t, 6> bitBoardCheckPairs = Board::bitBoardAnd(allyBitBoard,maskBitBoardPairs);

    const std::array<uint64_t, 6> maskBitBoardCheckQuads = Board::shift_right_board(bitBoardCheckPairs, dir * 2);
    const std::array<uint64_t, 6> bitBoardCheckQuads = Board::bitBoardAnd(bitBoardCheckPairs, maskBitBoardCheckQuads);

    const std::array<uint64_t, 6> maskBitBoardFinal = Board::shift_right_board(bitBoardCheckQuads, dir);
    std::array<uint64_t, 6> bitBoardFinal = Board::bitBoardAnd(bitBoardCheckQuads, maskBitBoardFinal);

    for (int i = 0; i < 6; i++) {
        while (bitBoardFinal[i] != 0) {
            const int startIndex = i * 64 + std::countr_zero(bitBoardFinal[i]);
            const std::array<int, 15> blockingIndexes = CaptureService::getBlockingCaptureIndex(allyBitBoard, enemyBitBoard, startIndex, dir);
            int blockingIndexIndex = 0;
            while (blockingIndexes[blockingIndexIndex] != -1) {
                if (nextIndex >= result.size()) {
                    return result;
                }
                result[nextIndex++] = blockingIndexes[blockingIndexIndex++];
            }
            bitBoardFinal[i] &= bitBoardFinal[i] - 1;
        }
    }
    return result;
}

bool AlignmentChecker::checkWinAt(const std::array<uint64_t, 6>& allyBB, int index) {
    int directions[] = {1, Board::SIZE + 1, Board::SIZE, Board::SIZE + 2};

    for (int dir : directions) {
        int count = 1;
        count += countLines(allyBB, index, dir);
        count += countLines(allyBB, index, -dir);
        if (count >= 5) return true;
    }
    return false;
}

int AlignmentChecker::countLines(const std::array<uint64_t, 6> &allyBitBoard, const int index, const int dir) {
    int result = 0;
    int currentIndex = index;

    for (int i = 0; i < 4; i++) {
        if (currentIndex < 0 || currentIndex >= Board::SIZE * (Board::SIZE + 1)) break;
        const int arrayIndex = currentIndex / 64;
        int bitPos = currentIndex % 64;
        if ((allyBitBoard[arrayIndex] & (1ULL << bitPos)) != 0) {
            result++;
            if (Board::isOutOfBounds(currentIndex, 1, dir)) break;
            currentIndex += dir;
        } else {
            break;
        }
    }
    return result;
}

Alignment AlignmentChecker::checkAlignment(const std::array<uint64_t, 4> &line) {
    bool patterIsFinish = false;
    bool gap_pending = false;
    Alignment result = {
        0,
        false,
        true,
        4
    };
    for (int i = 0; i < 4; i++) {
        if (line[i] == 0) {
            if (gap_pending) {
                gap_pending = false;
                patterIsFinish = true;
            }
            if (!patterIsFinish) {
                gap_pending = true;
            }
        }
        else if (line[i] == 1) {
            if (!patterIsFinish) {
                result.nbStone++;
            }
            if (gap_pending) {
                result.hasHole = true;
                gap_pending = false;
            }

        }
        else {
            result.blockDistance = i;
            result.isOpen = false;
            return result;
        }
    }
    return result;
}

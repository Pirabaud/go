#include "AlignmentChecker.hpp"

#include <algorithm>
#include <bitset>
#include <iostream>

#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"



bool AlignmentChecker::checkWinAlignment(const std::array<uint64_t, 6> &allyBitBoard, const std::array<uint64_t, 6> &enemyBitBoard, const int dir) {
    const std::array<uint64_t, 6> maskBitBoardPairs = Board::shift_right_board(allyBitBoard, dir);
    const std::array<uint64_t, 6> bitBoardCheckPairs = Board::bitBoardAnd(allyBitBoard,maskBitBoardPairs);

    const std::array<uint64_t, 6> maskBitBoardCheckQuads = Board::shift_right_board(bitBoardCheckPairs, dir * 2);
    const std::array<uint64_t, 6> bitBoardCheckQuads = Board::bitBoardAnd(bitBoardCheckPairs, maskBitBoardCheckQuads);

    const std::array<uint64_t, 6> maskBitBoardFinal = Board::shift_right_board(bitBoardCheckQuads, dir);
    std::array<uint64_t, 6> bitBoardFinal = Board::bitBoardAnd(bitBoardCheckQuads, maskBitBoardFinal);

    for (int i = 0; i < 6; i++) {
        while (bitBoardFinal[i] != 0) {
            if (!CaptureService::winLineBreakable(
                allyBitBoard,
                enemyBitBoard,
                i * 64 + std::countr_zero(bitBoardFinal[i]),
                dir)) return true;
            bitBoardFinal[i] &= bitBoardFinal[i] - 1;
        }
    }
    return false;
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

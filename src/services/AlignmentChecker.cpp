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

bool AlignmentChecker::checkWinAt(const std::array<uint64_t, 6>& allyBB, int index) {
    // 1. On identifie quel bloc de 64 bits contient l'index
    int block = index / 64;
    uint64_t bit = 1ULL << (index % 64);

    // 2. On définit les 4 directions (Horizontal, Vertical, Diagonales)
    // On peut utiliser tes constantes de direction (1, 20, 19, 21 par exemple)
    int directions[] = {1, 20, 19, 21};

    for (int dir : directions) {
        int count = 1; // La pierre qu'on vient de poser

        // On compte les pierres alliées dans un sens...
        count += countLines(allyBB, index, dir);
        // ...puis dans l'autre sens
        count += countLines(allyBB, index, -dir);

        if (count >= 5) return true; // VICTOIRE !
    }
    return false;
}

int AlignmentChecker::countLines(const std::array<uint64_t, 6> &allyBitBoard, const int index, const int dir) {
    int result = 0;
    int currentIndex = index;

    for (int i = 0; i < 4; i++) {
        if (currentIndex < 0 || currentIndex >= 361) break;
        const int arrayIndex = currentIndex / 64;
        int bitPos = currentIndex % 64;
        if ((allyBitBoard[arrayIndex] & (1ULL << bitPos)) != 0) {
            result++;
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

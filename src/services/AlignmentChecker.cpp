#include "AlignmentChecker.hpp"

#include <algorithm>
#include <bitset>
#include <iostream>

#include "CheckLegalMove.hpp"


bool AlignmentChecker::check5Alignment(const std::array<uint64_t, 6> &bitBoard, const int dir) {
    const std::array<uint64_t, 6> maskBitBoardPairs = Board::shift_right_board(bitBoard, dir);
    const std::array<uint64_t, 6> bitBoardCheckPairs = Board::bitBoardAnd(bitBoard,maskBitBoardPairs);

    const std::array<uint64_t, 6> maskBitBoardCheckQuads = Board::shift_right_board(bitBoardCheckPairs, dir * 2);
    const std::array<uint64_t, 6> bitBoardCheckQuads = Board::bitBoardAnd(bitBoardCheckPairs, maskBitBoardCheckQuads);

    const std::array<uint64_t, 6> maskBitBoardFinal = Board::shift_right_board(bitBoardCheckQuads, dir);
    const std::array<uint64_t, 6> bitBoardFinal = Board::bitBoardAnd(bitBoardCheckQuads, maskBitBoardFinal);

    for (int i = 0; i < 6; i++) {
        if (bitBoardFinal[i] != 0) {
            return true;
        }
    }
    return false;
}
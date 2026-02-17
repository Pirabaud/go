//
// Created by pirabaud on 2/17/26.
//

#include "MoveOrdering.hpp"

#include "Board.h"

std::vector<int> MoveOrdering::generateMoveOrder(const std::array<uint64_t, 6> &allyBoard, const std::array<uint64_t, 6> &enemyBoard) {
    std::vector<int> moves{};
    int empty = 0;

    std::array<uint64_t, 6> occupied{};
    for (int i = 0; i < 6; i++) {
        occupied[i] = currentBoard.getBitBoardWhite()[i] | currentBoard.getBitBoardBlack()[i];
        if (occupied[i]) {
            empty++;
        }
    }
    if (empty == 6) {
        moves.push_back(180);
        return moves;
    }

    std::array<uint64_t, 6> newMoves = newMoves();
    for (int i = 0; i < 6; i++) {
        uint64_t candidates = newMoves[i] & ~occupied[i];
        while (candidates != 0) {

        }
    }
}

std::array<uint64_t, 6> MoveOrdering::newMoves(const std::array<uint64_t, 6> &occupied) {
    const std::array<uint64_t, 6> right = Board::shift_right_board(occupied, 1);
    const std::array<uint64_t, 6> occupied_right = Board::bitBoardOr(occupied, right);
    const std::array<uint64_t, 6> left = Board::shift_left_board(occupied, 1);
    const std::array<uint64_t, 6> occupied_horizontal = Board::bitBoardOr(occupied_right, left);

    const std::array<uint64_t, 6> top = Board::shift_right_board(occupied_horizontal, 20);
    const std::array<uint64_t, 6> occupied_top = Board::bitBoardOr(occupied_horizontal, top);
    const std::array<uint64_t, 6> down = Board::shift_left_board(occupied_horizontal, 20);
    return Board::bitBoardOr(occupied_top, down);
}

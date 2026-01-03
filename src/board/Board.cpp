//
// Created by pirabaud on 6/19/25.
//

#include "Board.h"

#include <bitset>
#include <iostream>
#include "Position.hpp"

int Board::getWhiteCaptured() const {
    return this->whiteStoneCaptured;
}

int Board::getBlackCaptured() const {
    return this->blackStoneCaptured;
}

std::array<uint64_t, 6> &Board::getBitBoardWhite() {
    return this->bitBoardWhite;
}

const std::array<uint64_t, 6> &Board::getBitBoardWhite() const {
    return this->bitBoardWhite;
}

std::array<uint64_t, 6> &Board::getBitBoardBlack() {
    return this->bitBoardBlack;
}

const std::array<uint64_t, 6> &Board::getBitBoardBlack() const {
    return this->bitBoardBlack;
}

void Board::addStoneWhite(const Position pos) {
    const int global_index = pos.x * (SIZE + 1) + pos.y;
    this->bitBoardWhite[global_index / 64] |= 1ULL << (global_index % 64);
}

void Board::addStoneBlack(const Position pos) {
    const int global_index = pos.x * (SIZE + 1) + pos.y;
    this->bitBoardBlack[global_index / 64] |= 1ULL << (global_index % 64);
}

void Board::addCaptures(const bool forWhitePlayer, const int stoneCount) {
    if (forWhitePlayer) {
        this->whiteStoneCaptured += stoneCount;
    } else {
        this->blackStoneCaptured += stoneCount;
    }
}

std::array<uint64_t, 6> Board::shift_right_board(const std::array<uint64_t, 6> &currentBitboard, const int shift) {
    std::array<uint64_t, 6> board{};

    uint64_t carry = 0;
    for (int i = 5; i >= 0; i--) {
        const uint64_t nextCarry = currentBitboard[i] << (64 - shift);
        board[i] = currentBitboard[i] >> shift | carry;
        carry = nextCarry;
    }

    return board;
}

std::array<uint64_t, 6> Board::bitBoardAnd(const std::array<uint64_t, 6> &bitBoard1,
                                           const std::array<uint64_t, 6> &bitBoard2) {
    std::array<uint64_t, 6> result{};
    for (int i = 0; i < 6; i++) {
        result[i] = bitBoard1[i] & bitBoard2[i];
    }
    return result;
}

int Board::getGlobalIndex(const Position pos) {
    return pos.x * (SIZE + 1) + pos.y;
}

bool Board::isBitAt(const std::array<uint64_t, 6> &bitBoard, const int globalIndex) {
    if (globalIndex < 0 || globalIndex >= 384) return false;
    const int arrayIndex = globalIndex / 64;
    const int bitIndex = globalIndex % 64;
    return (bitBoard[arrayIndex] & (1ULL << bitIndex)) != 0;
}

void Board::clearBitAt(std::array<uint64_t, 6> &bitBoard, const int globalIndex) {
    const int arrayIndex = globalIndex / 64;
    const int bitIndex = globalIndex % 64;
    bitBoard[arrayIndex] &= ~(1ULL << bitIndex);
}

std::ostream &operator<<(std::ostream &os, const Board &board) {
    for (int i = 0; i < Board::SIZE; i++) {
        os << i;
        os << ':';
        for (int j = 0; j < Board::SIZE; j++) {
            if (Board::isBitAt(board.getBitBoardWhite(), Board::getGlobalIndex({i, j}))) {
                os << 'W';
            }
            else if (Board::isBitAt(board.getBitBoardBlack(), Board::getGlobalIndex({i, j}))) {
                os << 'B';
            }
            else {
                os << 'O';
            }
        }
        os << '\n';
    }
    return os;
}

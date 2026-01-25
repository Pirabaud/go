//
// Created by pirabaud on 6/19/25.
//

#include "Board.h"

#include <bitset>
#include <iomanip>
#include <iostream>
#include <random>

#include "HeuristicService.h"
#include "Position.hpp"

uint64_t Board::ZOBRIST_TABLE[361][2] = {};

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
    this->updateCurrentZobristKey(global_index, false);
}

void Board::addStoneBlack(const Position pos) {
    const int global_index = pos.x * (SIZE + 1) + pos.y;
    this->bitBoardBlack[global_index / 64] |= 1ULL << (global_index % 64);
    this->updateCurrentZobristKey(global_index, true);
}

void Board::addStoneWhite(const int index) {
    this->bitBoardWhite[index / 64] |= 1ULL << (index % 64);
    this->updateCurrentZobristKey(index, false);
}

void Board::addStoneBlack(const int index) {
    this->bitBoardBlack[index / 64] |= 1ULL << (index % 64);
    this->updateCurrentZobristKey(index, true);
}

void Board::removeWhiteStone(const int index) {
    this->bitBoardWhite[index / 64] &= ~(1ULL << (index % 64));
    this->updateCurrentZobristKey(index, false);
}

void Board::removeBlackStone(const int index) {
    this->bitBoardBlack[index / 64] &= ~(1ULL << (index % 64));
    this->updateCurrentZobristKey(index, true);
}

int Board::getPatternIndex(int positionIndex, bool isBlackPlayer, int direction) const {
    // direction : 1 (horizontal), 20 (vertical), 21 (Diag /), 19 (Diag \)
    int index = 0;
    // Use ALLY_BITS_MASK and ENEMY_BITS_MASK to build the index
    // Bitboards are of type std::array<uint64_t, 6>
    const std::array<uint64_t, 6> &allyBitBoard = isBlackPlayer ? this->bitBoardBlack : this->bitBoardWhite;
    const std::array<uint64_t, 6> &enemyBitBoard = isBlackPlayer ? this->bitBoardWhite : this->bitBoardBlack;

    for (int offset = -4; offset <= 4; offset++) {
        const int checkIndex = positionIndex + offset * direction;
        index <<= 2; // Make space for the next 2 bits
        if (isBitAt(allyBitBoard, checkIndex)) {
            index |= 0b01; // Ally stone
        } else if (isBitAt(enemyBitBoard, checkIndex)) {
            index |= 0b10; // Enemy stone
        } else {
            index |= 0b00; // Empty
        }
    }
    // Print index in binary
    return index;
}


void Board::addCaptures(const bool forWhitePlayer, const int stoneCount) {
    if (forWhitePlayer) {
        this->whiteStoneCaptured += stoneCount;
    } else {
        this->blackStoneCaptured += stoneCount;
    }
}

bool Board::isEmpty() const {
    for (int i = 0; i < 6; i++) {
        if (bitBoardWhite[i] != 0 || bitBoardBlack[i] != 0) {
            return false;
        }
    }
    return true;
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

std::array<uint64_t, 6> Board::shift_left_board(const std::array<uint64_t, 6> &currentBitboard, const int shift) {
    std::array<uint64_t, 6> board{};

    uint64_t carry = 0;
    for (int i = 0; i < 6; i++) {
        const uint64_t nextCarry = currentBitboard[i] >> (64 - shift);
        board[i] = (currentBitboard[i] << shift) | carry;
        carry = nextCarry;
    }

    return board;
}

std::array<uint64_t, 6> Board::bitBoardAnd(
    const std::array<uint64_t, 6> &bitBoard1,
    const std::array<uint64_t, 6> &bitBoard2) {
    std::array<uint64_t, 6> result{};
    for (int i = 0; i < 6; i++) {
        result[i] = bitBoard1[i] & bitBoard2[i];
    }
    return result;
}

std::array<uint64_t, 6> Board::bitBoardOr(
    const std::array<uint64_t, 6> &bitBoard1,
    const std::array<uint64_t, 6> &bitBoard2) {
    std::array<uint64_t, 6> result{};
    for (int i = 0; i < 6; i++) {
        result[i] = bitBoard1[i] | bitBoard2[i];
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

void Board::initZobrist() {
    std::__1::mt19937_64 rng(42); // Fixed seed for reproducibility
    std::uniform_int_distribution<uint64_t> dist;
    for (auto & indecesPosition : ZOBRIST_TABLE) {
        indecesPosition[0] = dist(rng); // Black stone
        indecesPosition[1] = dist(rng); // White stone
    }
}

void Board::updateCurrentZobristKey(int index, bool isBlack) {
    currentZobristKey ^= ZOBRIST_TABLE[index][isBlack ? 1 : 0];
}

std::ostream &operator<<(std::ostream &os, const Board &board) {
    for (int i = 0; i < Board::SIZE; i++) {
        os << std::setw(2) << std::setfill(' ') << i;
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

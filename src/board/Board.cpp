//
// Created by pirabaud on 6/19/25.
//

#include "Board.h"

#include <bitset>
#include <iostream>

#include "CaptureService.hpp"
#include "Position.hpp"


Board::Board() {
    fillEmptyGridLineAndCol(gridLineBlack);
    fillEmptyGridLineAndCol(gridLineWhite);
    fillEmptyGridLineAndCol(gridColBlack);
    fillEmptyGridLineAndCol(gridColWhite);
    fillEmptyGridDiag(gridDiagRightBlack);
    fillEmptyGridDiag(gridDiagRightWhite);
    fillEmptyGridDiag(gridDiagLeftBlack);
    fillEmptyGridDiag(gridDiagLeftWhite);
    for (unsigned long &i: bitBoardWhite) {
        i = 0;
    }
}


void Board::removeStoneCaptureAtPosition(StoneMask &enemyMask, const Position pos, const Position dir) {
    const auto stone1 = Position{pos.x + dir.x, pos.y + dir.y};
    const auto stone2 = Position{pos.x + dir.x * 2, pos.y + dir.y * 2};
    removeStoneAt(isWhiteStoneAt(stone1), stone1);
    removeStoneAt(isWhiteStoneAt(stone2), stone2);
    if (enemyMask == gridLineWhite) {
        whiteStoneCaptured += 2;
    } else {
        blackStoneCaptured += 2;
    }
}


void Board::fillEmptyGridLineAndCol(StoneMask &grid) {
    for (auto &row: grid) {
        row = 0;
    }
}

void Board::fillEmptyGridDiag(StoneMaskDiag &grid) {
    for (auto &row: grid) {
        row = 0;
    }
}

// void Board::emptyColumn(const int col) {
//     for (int row = 0; row < SIZE; row++) {
//         const auto pos = Position{col, row};
//             removeStoneAt(isWhiteStoneAt(pos), pos);
//     }
// }
// void Board::emptyLine(const int row) {
//     for (int col = 0; col < SIZE; col++) {
//         const auto pos = Position{row, col};
//         removeStoneAt(isWhiteStoneAt(pos), pos);
//     }
// }

void Board::printBoard() const {
    std::cout << "============BOARD=============" << std::endl;
    for (int row = 0; row < SIZE; row++) {
        std::cout << row << ": ";
        for (int col = 0; col < SIZE; col++) {
            if (isWhiteStoneAt(Position{row, col})) {
                std::cout << 'W';
            } else if (isBlackStoneAt(Position{row, col})) {
                std::cout << 'B';
            } else {
                std::cout << 'O';
            }
        }
        std::cout << std::endl;
    }
}

void Board::printLineAndCol(const StoneMask &grid) {
    std::cout << "============================" << std::endl;
    for (int row = 0; row < SIZE; row++) {
        std::cout << row << ": ";
        for (int col = 0; col < SIZE; col++) {
            if (isStoneOnLineAndDiagAt(grid, {row, col})) {
                std::cout << 'S';
            } else {
                std::cout << 'O';
            }
        }
        std::cout << std::endl;
    }
}

void Board::printDiag(const StoneMaskDiag &grid) {
    std::cout << "============================" << std::endl;
    for (int row = 0; row < SIZE * 2; row++) {
        std::cout << row << ": ";
        for (int col = 0; col < SIZE; col++) {
            if (isStoneOnDiagAt(grid, {row, col})) {
                std::cout << 'S';
            } else {
                std::cout << 'O';
            }
        }
        std::cout << std::endl;
    }
}

int Board::getWhiteCaptured() const {
    return this->whiteStoneCaptured;
}

int Board::getBlackCaptured() const {
    return this->blackStoneCaptured;
}

std::array<uint64_t, 6>& Board::getBitBoardWhite()  {
    return this->bitBoardWhite;
}

std::array<uint64_t, 6>& Board::getBitBoardBlack()  {
    return this->bitBoardBlack;
}

Board::StoneMask &Board::getLineGridWhite() {
    return this->gridLineWhite;
}

Board::StoneMask &Board::getLineGridBlack() {
    return this->gridLineBlack;
}

Board::StoneMask &Board::getColGridWhite() {
    return this->gridColWhite;
}

Board::StoneMask &Board::getColGridBlack() {
    return this->gridColBlack;
}

Board::StoneMaskDiag &Board::getDiagRightGridWhite() {
    return this->gridDiagRightWhite;
}

Board::StoneMaskDiag &Board::getDiagRightGridBlack() {
    return this->gridDiagRightBlack;
}

Board::StoneMaskDiag &Board::getDiagLeftGridWhite() {
    return this->gridDiagLeftWhite;
}

Board::StoneMaskDiag &Board::getDiagLeftGridBlack() {
    return this->gridDiagLeftBlack;
}

void Board::addStoneWhite(const Position pos) {
    const int global_index = pos.x * (SIZE + 1) + pos.y;
    this->bitBoardWhite[global_index / 64] |= 1ULL << (global_index % 64);
}

void Board::addStoneBlack(const Position pos) {

    const int global_index = pos.x * (SIZE + 1) + pos.y;
    this->bitBoardBlack[global_index / 64] |= 1ULL << (global_index % 64);
}

void Board::removeStoneAt(bool color, const Position pos) {
    if (color) {
        removeWhiteStoneAt(pos);
    } else {
        removeBlackStoneAt(pos);
    }
}

void Board::removeWhiteStoneAt(const Position pos) {
    const uint32_t rowBit = 1u << (SIZE - 1 - pos.y);
    this->getLineGridWhite().at(pos.x) &= ~rowBit;

    const uint32_t colBit = 1u << (SIZE - 1 - pos.x);
    this->getColGridWhite().at(pos.y) &= ~colBit;

    const int diagRightIndex = pos.x + pos.y;
    this->getDiagRightGridWhite().at(diagRightIndex) &= ~(1u << pos.x);

    const int diagLeftIndex = pos.x - pos.y + (SIZE - 1);
    this->getDiagLeftGridWhite().at(diagLeftIndex) &= ~(1u << pos.x);
}

void Board::removeBlackStoneAt(const Position pos) {
    const uint32_t rowBit = 1u << (SIZE - 1 - pos.y);
    this->getLineGridBlack().at(pos.x) &= ~rowBit;

    const uint32_t colBit = 1u << (SIZE - 1 - pos.x);
    this->getColGridBlack().at(pos.y) &= ~colBit;

    const int diagRightIndex = pos.x + pos.y;
    this->getDiagRightGridBlack().at(diagRightIndex) &= ~(1u << pos.x);

    const int diagLeftIndex = pos.x - pos.y + (SIZE - 1);
    this->getDiagLeftGridBlack().at(diagLeftIndex) &= ~(1u << pos.x);
}

void Board::save() {
    saveGridBlack.push_back(gridLineBlack);
    saveGridWhite.push_back(gridLineWhite);
    std::cout << saveGridBlack.size() << std::endl;
}

void Board::restore() {
    gridLineBlack = saveGridBlack.back();
    gridLineWhite = saveGridWhite.back();
    saveGridBlack.pop_back();
    saveGridWhite.pop_back();
}

std::ostream &operator<<(std::ostream &os, Board &board) {
    os << "grid black: \n";
    for (const auto &row: board.getLineGridBlack()) {
        os << std::bitset<Board::SIZE>(row) << '\n';
    }
    os << "grid White: \n";
    for (const auto &row: board.getLineGridWhite()) {
        os << std::bitset<Board::SIZE>(row) << '\n';
    }
    return os;
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
        //std::cout << std::bitset<64>(result[i]) << std::endl;
    }
    return result;
}

bool Board::isStoneOnLineAndDiagAt(const StoneMask &grid, const Position pos) {
    if (pos.x >= SIZE || pos.y >= SIZE) return false;
    const auto row = grid.at(pos.x);
    const auto stone = 1 << (SIZE - pos.y - 1);
    return (row & stone) != 0;
}

bool Board::isStoneOnDiagAt(const StoneMaskDiag &grid, Position pos) {
    if (pos.x >= SIZE * 2 || pos.y >= SIZE) return false;
    const auto row = grid.at(pos.x);
    const auto stone = 1 << (SIZE - pos.y - 1);
    return (row & stone) != 0;
}

bool Board::isBlackStoneAt(const Position pos) const {
    return isStoneOnLineAndDiagAt(gridLineBlack, pos);
}

bool Board::isWhiteStoneAt(const Position pos) const {
    return isStoneOnLineAndDiagAt(gridLineWhite, pos);
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

void Board::clearBitAt( std::array<uint64_t, 6> &bitBoard, const int globalIndex) {
    const int arrayIndex = globalIndex / 64;
    const int bitIndex = globalIndex % 64;
    bitBoard[arrayIndex] &= ~(1u << bitIndex);
}


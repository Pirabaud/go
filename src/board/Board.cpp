//
// Created by pirabaud on 6/19/25.
//

#include "Board.h"

#include <bitset>
#include <iostream>


Board::Board() {
    for (auto& row : this->gridBlack) {
        row = 0;
    }
    for (auto& row : this->gridWhite) {
        row = 0;
    }
}

Board::StoneMask& Board::getGridWhite() {
    return this->gridWhite;
}

Board::StoneMask& Board::getGridBlack() {
    return this->gridBlack;
}

void Board::addStoneWhite(const int& x, const int& y) {
    const uint32_t newStone = 1u << (SIZE - 1 - y);
    this->getGridWhite().at(x) = this->getGridWhite().at(x) | newStone;
}

void Board::addStoneBlack(const int& x, const int& y) {
    const uint32_t newStone = 1u << (SIZE - 1 - y);
    this->getGridBlack().at(x) = this->getGridBlack().at(x) | newStone;
}

void Board::removeStoneAt(StoneMask& mask, const int& x, const int& y) {
    const uint32_t stone = 1 << (SIZE - y - 1);
    const uint32_t antiMask = FULL_ROW ^ stone;
    mask.at(x) &= antiMask;
}

void Board::removeWhiteStoneAt(const int& rowIndex, const int& colIndex) {
    removeStoneAt(gridWhite, rowIndex, colIndex);
}

void Board::removeBlackStoneAt(const int& rowIndex, const int& colIndex) {
    removeStoneAt(gridBlack, rowIndex, colIndex);
}


std::ostream& operator<<(std::ostream& os, Board& board) {
    os << "grid black: \n";
    for (const auto& row : board.getGridBlack()) {
        os << std::bitset<Board::SIZE>(row) << '\n';
    }
    os << "grid White: \n";
    for (const auto& row : board.getGridWhite()) {
        os << std::bitset<Board::SIZE>(row) << '\n';
    }
    return os;
}

bool Board::isStoneAt(const StoneMask& grid, const int& rowIndex, const int& colIndex) {
    if (rowIndex >= Board::SIZE || colIndex >= Board::SIZE) return false;
    const auto row = grid.at(rowIndex);
    const auto stone = 1 << (SIZE - colIndex - 1);
    return (row & stone) != 0;
}

bool Board::isBlackStoneAt(const int& rowIndex, const int& colIndex) const {
    return isStoneAt(gridBlack, rowIndex, colIndex);
}

bool Board::isWhiteStoneAt(const int& rowIndex, const int& colIndex) const {
    return isStoneAt(gridWhite, rowIndex, colIndex);
}




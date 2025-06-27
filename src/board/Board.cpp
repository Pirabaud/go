//
// Created by pirabaud on 6/19/25.
//

#include "Board.h"

#include <bitset>
#include <iostream>


Board::Board() {
    for (auto & row : this->gridBlack) {
        row = 0;
    }
    for (auto & row : this->gridWhite) {
        row = 0;
    }

}

Board::StoneMask &Board::getGridWhite() {
    return this->gridWhite;
}

Board::StoneMask &Board::getGridBlack() {
    return this->gridBlack;
}

void Board::addStoneWhite(const int  &x, const int  &y) {
    const uint32_t newStone = 1u << SIZE - 1 - y;
    this->getGridWhite().at(x) = this->getGridWhite().at(x) | newStone;
}

void Board::addStoneBlack(const int  &x, const int  &y) {
    const uint32_t newStone = 1u << SIZE - 1 - y;
    this->getGridBlack().at(x) = this->getGridBlack().at(x) | newStone;
}

std::ostream & operator<<(std::ostream &os, Board &board) {
    os << "grid black: \n";
    for (const auto & row : board.getGridBlack()) {
        os << std::bitset<Board::SIZE>(row) << '\n';
    }
    os << "grid White: \n";
    for (const auto & row : board.getGridWhite()) {
        os << std::bitset<Board::SIZE>(row) << '\n';
    }
    return os;
}

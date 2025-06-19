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

std::array<uint32_t, Board::SIZE> Board::getGridWhite() const {
    return this->gridWhite;
}
uint32_t Board::getGridBlack() const {
    return this->getGridBlack();
}

std::ostream& operator<<(std::ostream& os, std::byte b)
{
    return os << std::bitset<Board::SIZE>(std::to_integer<int>(b));
}

std::ostream& operator<<(std::ostream& os, Board board){
    os << "grid black: \n";
    for (auto & row : board.getGridBlack()) {
        os << std::byte{row} << '\n';
    }
    os << "grid White: \n";
    for (auto & row : board.getGridWhite()) {
        os << std::byte{row} << '\n';
    }
    return os;
}

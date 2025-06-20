//
// Created by pirabaud on 6/19/25.
//

#include "Board.h"

#include <bitset>
#include <iostream>


Board::Board()
{
    for (auto& row : this->gridBlack)
    {
        row = 0;
    }
    for (auto& row : this->gridWhite)
    {
        row = 0;
    }
}

const Board::StoneMask& Board::getGridWhite() const
{
    return this->gridWhite;
}

const Board::StoneMask& Board::getGridBlack() const
{
    return this->gridBlack;
}

void Board::addStoneWhite(const int x, const int y)
{
    const uint32_t newStone = 1u << SIZE - 1 - y;
    this->gridWhite.at(x) = this->gridWhite.at(x) | newStone;
}

void Board::addStoneBlack(const int x, const int y)
{
    const uint32_t newStone = 1u << SIZE - 1 - y;
    this->gridBlack.at(x) = this->gridBlack.at(x) | newStone;
}

std::ostream& operator<<(std::ostream& os, const Board& board)
{
    os << "grid black: \n";
    for (auto& row : board.getGridBlack())
    {
        os << std::bitset<Board::SIZE>(row) << '\n';
    }
    os << "grid White: \n";
    for (auto& row : board.getGridWhite())
    {
        os << std::bitset<Board::SIZE>(row) << '\n';
    }
    return os;
}

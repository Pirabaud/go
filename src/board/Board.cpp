//
// Created by pirabaud on 6/19/25.
//

#include "Board.h"

#include <bitset>
#include <iostream>

#include "CaptureService.hpp"
#include "Position.hpp"


Board::Board() {
    for (auto& row : this->gridBlack) {
        row = 0;
    }
    for (auto& row : this->gridWhite) {
        row = 0;
    }
}

bool Board::resolveCaptureAtPosition(const Position pos) {
    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto& [x, y] : directions) {
        const bool result = resolveCaptureAtPositionInDirection(pos, Position{x, y});
        if (result) {
            return true;
        }
    }
    return false;
}

void Board::removeStoneCaptureAtPosition(const Position pos, const Position dir, bool isBlack ) {
    if (isBlack) {
        removeBlackStoneAt({pos.x + dir.x, pos.y + dir.y});
        removeBlackStoneAt({pos.x + dir.x * 2, pos.y + dir.y * 2});
    } else {
        removeWhiteStoneAt({pos.x + dir.x, pos.y + dir.y});
        removeWhiteStoneAt({pos.x + dir.x * 2, pos.y + dir.y * 2});
    }
}

bool Board::resolveCaptureAtPositionInDirection(const Position pos, const Position dir) {
    // Check overflow
    if (pos.x + dir.x > SIZE - 1 || pos.x + dir.x < 0 ||
        pos.x + dir.x * 2 > SIZE - 1 || pos.x + dir.x * 2 < 0 ||
        pos.x + dir.x * 3 > SIZE - 1 || pos.x + dir.x * 3 < 0 ||
        pos.y + dir.y > SIZE - 1 || pos.y + dir.y < 0 ||
        pos.y + dir.y * 2 > SIZE - 1 || pos.y + dir.y * 2 < 0 ||
        pos.y + dir.y * 3 > SIZE - 1 || pos.y + dir.y * 3 < 0) {
        return false;
        }

    const StoneMask allyMask = isWhiteStoneAt(pos) ? getGridWhite() : getGridBlack();
    StoneMask& enemyMask = isWhiteStoneAt(pos) ? getGridBlack() : getGridWhite();

    const bool ex1 = isStoneAt(enemyMask, Position{pos.x + dir.x,pos.y + dir.y});
    const bool ex2 = isStoneAt(enemyMask, Position{pos.x + dir.x * 2, pos.y + dir.y * 2});
    const bool ex3 = isStoneAt(allyMask, Position{pos.x + dir.x * 3, pos.y + dir.y * 3});
    if (ex1 && ex2 && ex3) {
        removeStoneCaptureAtPosition(pos, dir, isBlackStoneAt(pos));
        return true;
    }
    return false;
}

void Board::emptyColumn(const int col) {
    for (int row = 0; row < SIZE; row++) {
        if (isWhiteStoneAt(Position{col, row}))
            removeWhiteStoneAt({col, row});
        else if (isBlackStoneAt(Position{col, row}))
            removeBlackStoneAt({col, row});
    }
}

void Board::emptyLine(const int row) {
    for (int col = 0; col < SIZE; col++) {
        if (isWhiteStoneAt(Position{col, row}))
            removeWhiteStoneAt({col, row});
        else if (isBlackStoneAt(Position{col, row}))
            removeBlackStoneAt({col, row});
    }
}

void Board::printBoard() const {
    std::cout << "============BOARD=============" << std::endl;
    for (int row = 0; row < SIZE; row++) {
        std::cout << row << ": ";
        for (int col = 0; col < SIZE; col++) {
            if (isWhiteStoneAt(Position{row, col})) {
                std::cout << 'W';
            }
            else if (isBlackStoneAt(Position{row, col})) {
                std::cout << 'B';
            }
            else {
                std::cout << 'O';
            }
        }
        std::cout << std::endl;
    }
}

Board::StoneMask& Board::getGridWhite(){
    return this->gridWhite;
}

Board::StoneMask& Board::getGridBlack(){
    return this->gridBlack;
}

int Board::getNumberOfBlackStones() const {
    return numberOfBlackStones;
}

int Board::getNumberOfWhiteStones() const {
    return numberOfWhiteStones;
}


void Board::addStoneWhite(const Position pos) {
    const uint32_t newStone = 1u << (SIZE - 1 - pos.y);
    this->getGridWhite().at(pos.x) = this->getGridWhite().at(pos.x) | newStone;
    ++numberOfWhiteStones;
}

void Board::addStoneBlack(const Position pos) {
    const uint32_t newStone = 1u << (SIZE - 1 - pos.y);
    this->getGridBlack().at(pos.x) = this->getGridBlack().at(pos.x) | newStone;
    ++numberOfBlackStones;
}

void Board::removeStoneAt(StoneMask& mask, const Position pos) {
    const uint32_t stone = 1 << (SIZE - pos.y - 1);
    const uint32_t antiMask = FULL_ROW ^ stone;
    mask.at(pos.x) &= antiMask;
}

void Board::removeWhiteStoneAt(const Position pos) {
    removeStoneAt(gridWhite, pos);
    --numberOfWhiteStones;
}

void Board::removeBlackStoneAt(const Position pos) {
    removeStoneAt(gridBlack,pos);
    --numberOfBlackStones;
}

void Board::resolveCaptures()  {
    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            if (isStoneAt(gridBlack, Position{x, y}) || isStoneAt(gridWhite, Position{x, y})) {
                resolveCaptureAtPosition(Position{x, y});
            }
        }
    }
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

bool Board::isStoneAt(const StoneMask& grid, const Position pos) {
    if (pos.x >= SIZE || pos.y >= SIZE) return false;
    const auto row = grid.at(pos.x);
    const auto stone = 1 << (SIZE - pos.y - 1);
    return (row & stone) != 0;
}

bool Board::isBlackStoneAt(const Position pos) const {
    return isStoneAt(gridBlack, pos);
}

bool Board::isWhiteStoneAt(const Position pos) const {
    return isStoneAt(gridWhite, pos);
}




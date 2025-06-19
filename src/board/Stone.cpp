//
// Created by pirabaud on 6/19/25.
//

#include "Stone.h"


int Stone::getCol() const {
    return this->row;
}

int Stone::getRow() const {
    return this->col;
}

Stone::Stone(const int row, const int col) {
    this->row = row;
    this->col = col;
}

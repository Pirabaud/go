#include "MinMax.hpp"

int MinMax::MAX_DEPTH = 3;

MinMax::MinMax(Board &board) : board(board) {
}

MinMax::~MinMax() {
    delete root;
}

Board & MinMax::getBoard() {
    return board;
}

Position MinMax::run() const {
}

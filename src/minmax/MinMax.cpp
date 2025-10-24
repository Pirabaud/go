#include "MinMax.hpp"

int MinMax::MAX_DEPTH = 10;

MinMax::MinMax(Board &board) : board(board) {
}

MinMax::~MinMax() {
}

Board & MinMax::getBoard() const {
    return board;
}

Position MinMax::run() const {
}

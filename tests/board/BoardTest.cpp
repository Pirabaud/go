#include "Board.h"
#include "catch2/catch_test_macros.hpp"

struct BoardTestFixture {
    Board board;

    void addAndCheckStone(Position pos) {
        board.addStoneBlack(pos);
        board.addStoneWhite(pos);
    }
};


TEST_CASE_METHOD(BoardTestFixture, "Board - Check Stone Placement and Stone Verification") {
    for (int i = 0; i < Board::SIZE - 1; ++i) {
        for (int j = 0; j < Board::SIZE - 1; ++j) {
            addAndCheckStone (Position{i, j});
        }
    }
}


TEST_CASE("Board - Check out of bounds check") {
    const Board board;
}

TEST_CASE("Board - Remove stone at position") {
    Board whiteBoard;
    Board blackBoard;
}
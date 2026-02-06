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

TEST_CASE("Board - Get pattern index (horizontal)") {
    Board board;
    board.addStoneWhite({5, 5});
    board.addStoneBlack({5, 6});
    board.addStoneWhite({5, 8});
    board.addStoneBlack({5, 9});
    board.addStoneWhite({5, 12});
    const int patternIndex = board.getPatternIndex(Board::getGlobalIndex({5, 9}), true, 1);
    REQUIRE(patternIndex == 0b0010010010010000100000);
}

TEST_CASE("Board - Get pattern index (vertical)") {
    Board board;
    board.addStoneWhite({ 5, 5});
    board.addStoneBlack({ 6, 5});
    board.addStoneWhite({ 8, 5});
    board.addStoneBlack({ 9, 5});
    board.addStoneWhite({ 12, 5});
    const int patternIndex = board.getPatternIndex(Board::getGlobalIndex({9, 5}), true, 20);
    REQUIRE(patternIndex == 0b0010010010010000100000);
}
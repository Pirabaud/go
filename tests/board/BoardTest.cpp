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

TEST_CASE("Board - Get pattern index (diagonal down-right)") {
    Board board;
    board.addStoneWhite({ 0, 0});
    board.addStoneBlack({ 1, 1});
    board.addStoneWhite({ 3, 3});
    board.addStoneBlack({ 5, 5});
    board.addStoneWhite({ 8, 8});;
    const int patternIndex = board.getPatternIndex(Board::getGlobalIndex({3, 3}), false, 21);
    REQUIRE(patternIndex == 0b1111011000010010000001);
}

TEST_CASE("Board - Get pattern index (diagonal up-left)") {
    Board board;
    board.addStoneWhite({ 0, 18});
    board.addStoneBlack({ 1, 17});
    board.addStoneWhite({ 3, 15});
    board.addStoneBlack({ 5, 13});
    board.addStoneWhite({ 8, 10});;
    const int patternIndex = board.getPatternIndex(Board::getGlobalIndex({3, 15}), false, 19);
    REQUIRE(patternIndex == 0b1111011000010010000001);
}
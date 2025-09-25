#include "Board.h"
#include "catch2/catch_test_macros.hpp"

struct BoardTestFixture {
    Board board;

    void addAndCheckStone(Position pos) {
        board.addStoneBlack(pos);
        board.addStoneWhite(pos);

        REQUIRE(board.isBlackStoneAt(pos));
        REQUIRE(board.isWhiteStoneAt( pos));
        REQUIRE_FALSE(board.isBlackStoneAt (Position{pos.x + 1, pos.y + 1}));
        REQUIRE_FALSE(board.isWhiteStoneAt (Position{pos.x + 1, pos.y + 1}));
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
    REQUIRE_FALSE(board.isBlackStoneAt (Position{Board::SIZE + 1, Board::SIZE + 1}));
    REQUIRE_FALSE(board.isWhiteStoneAt (Position{Board::SIZE + 1, Board::SIZE + 1}));
}

TEST_CASE("Board - Remove stone at position") {
    Board whiteBoard;
    Board blackBoard;
    for (int i = 0; i < Board::SIZE - 1; ++i) {
        for (int j = 0; j < Board::SIZE - 1; ++j) {
            whiteBoard.addStoneWhite (Position{i, j});
            blackBoard.addStoneBlack (Position{i, j});

            whiteBoard.removeWhiteStoneAt (Position{i, j});
            blackBoard.removeBlackStoneAt (Position{i, j});

            REQUIRE_FALSE(whiteBoard.isWhiteStoneAt (Position{i, j}));
            REQUIRE_FALSE(blackBoard.isBlackStoneAt (Position{i, j}));
        }
    }
}

TEST_CASE("Board - Test save and restore methods") {
    Board board;

    board.addStoneBlack({1, 1});
    board.addStoneWhite({1, 2});
    const Board::StoneMask whiteMask = board.getGridWhite();
    const Board::StoneMask blackMask = board.getGridBlack();
    board.save();
    board.addStoneBlack({2, 2});
    board.addStoneWhite({2, 3});
    REQUIRE_FALSE(board.getGridBlack() == blackMask);
    REQUIRE_FALSE(board.getGridWhite() == whiteMask);
    board.restore();
    REQUIRE(board.getGridBlack() == blackMask);
    REQUIRE(board.getGridWhite() == whiteMask);
}
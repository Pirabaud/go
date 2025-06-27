#include "Board.h"
#include "catch2/catch_test_macros.hpp"

struct BoardTestFixture {
    Board board;

    void addAndCheckStone(const int& rowIndex, const int& colIndex) {
        board.addStoneBlack(rowIndex, colIndex);
        board.addStoneWhite(rowIndex, colIndex);

        REQUIRE(board.isBlackStoneAt(rowIndex,colIndex));
        REQUIRE(board.isWhiteStoneAt( rowIndex,colIndex));
        REQUIRE_FALSE(board.isBlackStoneAt(rowIndex + 1,colIndex + 1));
        REQUIRE_FALSE(board.isWhiteStoneAt(rowIndex + 1,colIndex + 1));
    }
};


TEST_CASE_METHOD(BoardTestFixture, "Board - Check Stone Placement and Stone Verification") {
    for (int i = 0; i < Board::SIZE - 1; ++i) {
        for (int j = 0; j < Board::SIZE - 1; ++j) {
            addAndCheckStone(i, j);
        }
    }
}


TEST_CASE("Board - Check out of bounds check") {
    const Board board;
    REQUIRE_FALSE(board.isBlackStoneAt(Board::SIZE + 1,Board::SIZE + 1));
    REQUIRE_FALSE(board.isWhiteStoneAt(Board::SIZE + 1, Board::SIZE + 1));
}

TEST_CASE("Board - Remove stone at position") {
    Board whiteBoard;
    Board blackBoard;
    for (int i = 0; i < Board::SIZE - 1; ++i) {
        for (int j = 0; j < Board::SIZE - 1; ++j) {
            whiteBoard.addStoneWhite(i, j);
            blackBoard.addStoneBlack(i, j);

            whiteBoard.removeWhiteStoneAt(i, j);
            blackBoard.removeBlackStoneAt(i, j);

            REQUIRE_FALSE(whiteBoard.isWhiteStoneAt(i,j));
            REQUIRE_FALSE(blackBoard.isBlackStoneAt(i,j));
        }
    }
}

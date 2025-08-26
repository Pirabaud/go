#include <iostream>

#include "Board.h"
#include "CheckWinService.hpp"
#include "SFML/Graphics/Color.hpp"
#include "catch2/catch_test_macros.hpp"

struct CheckWinFixture {
    Board board;
};

TEST_CASE_METHOD(CheckWinFixture, "Check Win Service - No win condition") {
    REQUIRE(CheckWinService::isWin(board) == nullptr);
}

TEST_CASE_METHOD(CheckWinFixture, "Check Win Service - Wins horizontally") {
    for (int row = 0; row < Board::SIZE; ++row) {
        for (int col = 0; col < Board::SIZE - 4; ++col) {
            Board whiteBoard;
            Board blackBoard;
            for (int i = col; i < col + 5; ++i) {
                whiteBoard.addStoneWhite(Position(row, i));
                blackBoard.addStoneBlack(Position(row, i));
            }
            REQUIRE(CheckWinService::isWin(whiteBoard) == &sf::Color::White);
            REQUIRE(CheckWinService::isWin(blackBoard) == &sf::Color::Black);
        }
    }
}


TEST_CASE_METHOD(CheckWinFixture, "Check Win Service - Wins vertically") {
    for (int col = 0; col < Board::SIZE; ++col) {
        for (int row = 0; row < Board::SIZE - 4; ++row) {
            Board whiteBoard;
            Board blackBoard;
            for (int i = row; i < row + 5; ++i) {
                whiteBoard.addStoneWhite(Position(i, col));
                blackBoard.addStoneBlack(Position(i, col));
            }
            REQUIRE(CheckWinService::isWin(whiteBoard) == &sf::Color::White);
            REQUIRE(CheckWinService::isWin(blackBoard) == &sf::Color::Black);
        }
    }
}

TEST_CASE_METHOD(CheckWinFixture, "Check Win Service - Wins diagonally (top-left to bottom-right)") {
    for (int row = 0; row < Board::SIZE - 4; ++row) {
        for (int col = 0; col < Board::SIZE - 4; ++col) {
            Board whiteBoard;
            Board blackBoard;
            for (int i = 0; i < 5; ++i) {
                whiteBoard.addStoneWhite(Position(row + i, col + i));
                blackBoard.addStoneBlack(Position(row + i, col + i));
            }
            REQUIRE(CheckWinService::isWin(whiteBoard) == &sf::Color::White);
            REQUIRE(CheckWinService::isWin(blackBoard) == &sf::Color::Black);
        }
    }
}

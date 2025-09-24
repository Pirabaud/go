#include "Board.h"
#include "CheckMoveService.hpp"
#include "FreeThreeService.h"
#include "catch2/catch_test_macros.hpp"

struct BoardFixture {
    Board board;

    void setupCreateBoardWhite(const Position pos1, const Position& pos2, const Position& pos3, const bool expected){
        board = Board();

        if (CheckMoveService::notInBoard(pos1),
            CheckMoveService::notInBoard(pos2),
            CheckMoveService::notInBoard(pos3)) {
            return;
        }
        board.addStoneWhite(pos1);
        board.addStoneWhite(pos2);
        board.addStoneWhite(pos3);

        REQUIRE(FreeThreeService::isFreeThree(board, pos1) == expected);
    }

    void setupCreateBoardBlack(const Position pos1, const Position& pos2, const Position& pos3, const bool expected){
        board = Board();

        if (CheckMoveService::notInBoard(pos1),
            CheckMoveService::notInBoard(pos2),
            CheckMoveService::notInBoard(pos3)) {
            return;
            }
        board.addStoneBlack(pos1);
        board.addStoneBlack(pos2);
        board.addStoneBlack(pos3);

        REQUIRE(FreeThreeService::isFreeThree(board, pos1) == expected);
    }
};

TEST_CASE_METHOD(BoardFixture, "Check free three line right white") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 1; y < Board::SIZE - 3; y++) {
            setupCreateBoardWhite(Position(x, y), Position(x, y + 1), Position(x, y + 2), true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check free three line right with hole in 1 case white") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 1; y < Board::SIZE - 4; y++) {
            setupCreateBoardWhite(Position(x, y), Position(x, y + 2), Position(x, y + 3), true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check free three line right with hole in 2 case white") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 1; y < Board::SIZE - 4; y++) {
            setupCreateBoardWhite(Position(x, y), Position(x, y + 1), Position(x, y + 3), true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check free three line left white") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = Board::SIZE - 3; y > 3; y--) {
            setupCreateBoardWhite(Position(x, y), Position(x, y - 1), Position(x, y - 2), true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check free three line left with hole in 1 case white") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = Board::SIZE - 3; y > 3; y--) {
            setupCreateBoardWhite(Position(x, y), Position(x, y - 2), Position(x, y - 3), true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check free three line left with hole in 2 case white") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = Board::SIZE - 3; y > 3; y--) {
            setupCreateBoardWhite(Position(x, y), Position(x, y - 1), Position(x, y - 3), true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check free three line left black") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = Board::SIZE - 3; y > 3; y--) {
            setupCreateBoardBlack(Position(x, y), Position(x, y - 1), Position(x, y - 2), true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check free three line left with hole in 1 case black") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = Board::SIZE - 3; y > 3; y--) {
            setupCreateBoardBlack(Position(x, y), Position(x, y - 2), Position(x, y - 3), true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check free three line left with hole in 2 case black") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = Board::SIZE - 3; y > 3; y--) {
            setupCreateBoardBlack(Position(x, y), Position(x, y - 1), Position(x, y - 3), true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check free three line right black") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 1; y < Board::SIZE - 3; y++) {
            setupCreateBoardBlack(Position(x, y), Position(x, y + 1), Position(x, y + 2), true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check free three line right with hole in 1 case black") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 1; y < Board::SIZE - 3; y++) {
            setupCreateBoardBlack(Position(x, y), Position(x, y + 1), Position(x, y + 2), true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check free three line right with hole in 2 case black") {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 1; y < Board::SIZE - 3; y++) {
            setupCreateBoardBlack(Position(x, y), Position(x, y + 1), Position(x, y + 2), true);
        }
    }
}

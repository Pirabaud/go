//
// Created by pirabaud on 6/24/25.
//

#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "catch2/benchmark/catch_benchmark.hpp"
#include "IllegalMoves.hpp"
#include "CheckLegalMove.hpp"

bool isValidPosition(int x, int y) {
    return x >= 0 && x < Board::SIZE && y >= 0 && y < Board::SIZE;
}


struct BoardFixture {
    Board board;

    void setupCreatesCaptureTest(const Position posPlay,
                                 const Position posColor,
                                 const Position posOppColor1,
                                 const Position posOppColor2,
                                 const IllegalMoves::Type &expected) {
        board = Board();



        board.addStoneWhite(posColor);
        board.addStoneBlack(posOppColor1);
        board.addStoneBlack(posOppColor2);

        REQUIRE(
            CheckLegalMove::isLegalMove(posPlay, board, false) == expected);
    }

    void setupCheckAlreadyInBoard(Position pos, bool color) {
        board = Board();

        if (color) {
            board.addStoneBlack(pos);
        } else {
            board.addStoneWhite(pos);
        }
        CHECK(
            CheckLegalMove::isLegalMove(pos, board, true)
            == IllegalMoves::OCCUPIED);

        CHECK(
            CheckLegalMove::isLegalMove(pos,board, false)
            == IllegalMoves::OCCUPIED);
    }

    void setupDoubleFreeThree(bool color) {
        board = Board();

        if (color) {
            board.addStoneWhite({1, 1});
            board.addStoneWhite({2, 2});
            board.addStoneWhite({4, 5});
            board.addStoneWhite({4, 6});
            CHECK(CheckLegalMove::isLegalMove({4, 4}, board, false)
                == IllegalMoves::DOUBLE_FREE_CAPTURE);
        }
        else {
            board.addStoneBlack({1, 1});
            board.addStoneBlack({2, 2});
            board.addStoneBlack({4, 5});
            board.addStoneBlack({4, 6});
            CHECK(CheckLegalMove::isLegalMove({4, 4}, board, true)
                == IllegalMoves::DOUBLE_FREE_CAPTURE);
        }
    }

    void setupNotDoubleFreeThree(bool color) {
        board = Board();

        if (color) {
            board.addStoneWhite({1, 1});
            board.addStoneWhite({2, 2});
            board.addStoneWhite({4, 5});
            board.addStoneWhite({4, 6});
            board.addStoneBlack({4, 3});
            CHECK(CheckLegalMove::isLegalMove({4, 4}, board, false)
                == IllegalMoves::NONE);
        }
        else {
            board.addStoneBlack({1, 1});
            board.addStoneBlack({2, 2});
            board.addStoneBlack({4, 5});
            board.addStoneBlack({4, 6});
            board.addStoneWhite({4, 3});
            CHECK(CheckLegalMove::isLegalMove({4, 4}, board, true)
                == IllegalMoves::NONE);
        }
    }
};


TEST_CASE_METHOD(BoardFixture, "Check creating capture col down") {
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            setupCreatesCaptureTest (Position{x, y},
                                    Position{x, y + 1},
                                    Position{x, y - 1},
                                    Position{x, y + 2},
                                    IllegalMoves::Type::CREATE_CAPTURE);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check creating capture col up") {
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            setupCreatesCaptureTest (Position{x, y},
                                    Position{x, y - 1},
                                    Position{x, y + 1},
                                    Position{x, y - 2},
                                    IllegalMoves::Type::CREATE_CAPTURE);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check creating capture line right") {
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            setupCreatesCaptureTest (Position{x, y},
                                    Position{x + 1, y},
                                    Position{x - 1, y},
                                    Position{x + 2, y},
                                    IllegalMoves::Type::CREATE_CAPTURE);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check creating capture line lef") {
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            setupCreatesCaptureTest (Position{x, y},
                        Position{x - 1, y},
                        Position{x + 1, y},
                        Position{x - 2, y},
                        IllegalMoves::Type::CREATE_CAPTURE);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check creating capture diagonal top right") {
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            setupCreatesCaptureTest (Position{x, y},
                        Position{x - 1, y + 1},
                        Position{x + 1, y - 1},
                        Position{x - 2, y + 2},
                        IllegalMoves::Type::CREATE_CAPTURE);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check creating capture diagonal bot left") {
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            setupCreatesCaptureTest (Position{x, y},
                        Position{x + 1, y - 1},
                        Position{x - 1, y + 1},
                        Position{x + 2, y - 2},
                        IllegalMoves::Type::CREATE_CAPTURE);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check creating capture diagonal top left") {
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            setupCreatesCaptureTest (Position{x, y},
                        Position{x - 1, y - 1},
                        Position{x + 1, y + 1},
                        Position{x - 2, y - 2},
                        IllegalMoves::Type::CREATE_CAPTURE);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check creating capture diagonal bot right") {
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            setupCreatesCaptureTest (Position{x, y},
                     Position{x + 1, y + 1},
                     Position{x - 1, y - 1},
                     Position{x + 2, y + 2},
                     IllegalMoves::Type::CREATE_CAPTURE);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check stone already in board for black") {
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            setupCheckAlreadyInBoard (Position{x, y}, true);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check stone already in board for white") {
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            setupCheckAlreadyInBoard (Position{x, y}, false);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check double free three") {
    setupDoubleFreeThree(false);
    setupDoubleFreeThree(true);
}

TEST_CASE_METHOD(BoardFixture, "Check not double free three") {
    setupNotDoubleFreeThree(false);
    setupNotDoubleFreeThree(true);
}

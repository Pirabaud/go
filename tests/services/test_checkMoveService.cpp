//
// Created by pirabaud on 6/24/25.
//

#include <catch2/catch_test_macros.hpp>

#include "catch2/benchmark/catch_benchmark.hpp"
#include "IllegalMoves.hpp"
#include "CheckMoveService.hpp"

struct BoardFixture {
    Board board;

    void setupCreatesCaptureTest(
        const int& playX,
        const int& playY,
        const int& playColorX,
        const int& playColorY,
        const int& oppositeColor1X,
        const int& oppositeColor1Y,
        const int& oppositeColor2X,
        const int& oppositeColor2Y,
        const IllegalMoves::Type& expected) {
        board.addStoneWhite(playColorX, playColorY);
        board.addStoneBlack(oppositeColor1X, oppositeColor1Y);
        board.addStoneBlack(oppositeColor2X, oppositeColor2Y);

        REQUIRE(
            CheckMoveService::isLegalMove(playX, playY, board.getGridBlack(), board.getGridWhite(), false) == expected);
    }
};

TEST_CASE_METHOD(BoardFixture, "Check Creating Capture") {
    SECTION("Check creating capture line left") {
        setupCreatesCaptureTest(0, 2, 0, 1, 0, 0, 0, 3, IllegalMoves::Type::CREATE_CAPTURE);
    }

    SECTION("Check creating capture line right") {
        setupCreatesCaptureTest(0, 1, 0, 2, 0, 0, 0, 3, IllegalMoves::Type::CREATE_CAPTURE);
    }

    SECTION("Check creating capture col bot") {
        setupCreatesCaptureTest(1, 0, 2, 0, 0, 0, 3, 0, IllegalMoves::Type::CREATE_CAPTURE);
    }

    SECTION("Check creating capture col top") {
        setupCreatesCaptureTest(2, 0, 1, 0, 0, 0, 3, 0, IllegalMoves::Type::CREATE_CAPTURE);
    }

    SECTION("Check creating capture diagonal top right") {
        setupCreatesCaptureTest(2, 1, 1, 2, 3, 0, 0, 3, IllegalMoves::Type::CREATE_CAPTURE);
    }

    SECTION("Check creating capture diagonal bot left") {
        setupCreatesCaptureTest(2, 1, 1, 2, 3, 0, 0, 3, IllegalMoves::Type::CREATE_CAPTURE);
    }

    SECTION("already stone on this pos") {
        setupCreatesCaptureTest(1, 2, 1, 2, 3, 0, 0, 3, IllegalMoves::Type::OCCUPIED);
    }

    SECTION("out of board") {
        setupCreatesCaptureTest(-2, 1, 1, 2, 3, 0, 0, 3, IllegalMoves::Type::NOT_IN_BOARD);
    }
}


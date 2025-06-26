//
// Created by pirabaud on 6/24/25.
//

#include <catch2/catch_test_macros.hpp>
#include "services/CheckMoveService.h"

struct boardFixture {
    Board board;

    void setupCaptureTest(
        const int playX,
        const int playY,
        const int playColorX,
        const int playColorY,
        const int oppositeColor1X,
        const int oppositeColor1Y,
        const int oppositeColor2X,
        const int oppositeColor2Y,
        const bool expected) {
        board.addStoneWhite(playColorX, playColorY);
        board.addStoneBlack(oppositeColor1X, oppositeColor1Y);
        board.addStoneBlack(oppositeColor2X, oppositeColor2Y);

        REQUIRE(CheckMoveService::isLegalMove(playX, playY, board.getGridBlack(), board.getGridWhite(), false) == expected);
    }
};

TEST_CASE_METHOD(boardFixture, "Check Capture") {
    SECTION("Check capture line left") {
        setupCaptureTest(0, 2, 0, 1, 0, 0, 0, 3, false);
    }

    SECTION("Check capture line right") {
        setupCaptureTest(0, 1, 0, 2, 0, 0, 0, 3, false);
    }

    SECTION("Check capture col bot") {
        setupCaptureTest(1, 0, 2, 0, 0, 0, 3, 0, false);
    }

    SECTION("Check capture col top") {
        setupCaptureTest(2, 0, 1, 0, 0, 0, 3, 0, false);
    }

    SECTION("Check capture diagonal top right") {
        setupCaptureTest(2, 1, 1, 2, 3, 0, 0, 3, false);
    }

    SECTION("Check capture diagonal bot left") {
        setupCaptureTest(2, 1, 1, 2, 3, 0, 0, 3, false);
    }

    SECTION("already stone on this pos") {
        setupCaptureTest(2, 1, 1, 2, 3, 0, 0, 3, false);
    }

    SECTION("out of board") {
        setupCaptureTest(-2, 1, 1, 2, 3, 0, 0, 3, false);
    }
}


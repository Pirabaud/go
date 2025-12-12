#include <bitset>
#include <iostream>
#include "catch2/catch_test_macros.hpp"
#include "CheckLegalMove.hpp"
#include "catch2/internal/catch_stdstreams.hpp"

std::array directions = {
    Position(0, 1),
    Position(1, 0),
    Position(1, -1),
    Position(1, 1)
};


TEST_CASE("ALIGNMENT - Check stone alone") {
    Board board;

    board.addStoneBlack({9, 9});

    for (auto& dir : directions) {
        Alignment alignment = AlignmentChecker::detectAlignment({9, 9}, board.getLineGridBlack(), board.getLineGridWhite(), dir);
        REQUIRE(alignment.nbRealAlignment == 1);
        REQUIRE(alignment.nbPotentialAlignment == 1);
        REQUIRE(alignment.holeCount == 8);
        REQUIRE(alignment.blockDistanceLeft == 0);
        REQUIRE(alignment.blockDistanceRight == 0);
   }
}

TEST_CASE("ALIGNMENT - Alternate stones") {
    Board board;

    // WBOBOBOBOB
    board.addStoneWhite({9, 4});
    board.addStoneBlack({9, 5});
    board.addStoneBlack({9, 7});
    board.addStoneBlack({9, 9});
    board.addStoneBlack({9, 11});
    board.addStoneBlack({9, 13});

    Alignment alignment = AlignmentChecker::detectAlignment({9, 9}, board.getLineGridBlack(), board.getLineGridWhite(), {0, 1});
    REQUIRE(alignment.nbRealAlignment == 2);
    REQUIRE(alignment.nbPotentialAlignment == 5);
    REQUIRE(alignment.holeCount == 4);
    REQUIRE(alignment.blockDistanceRight == 0);
    REQUIRE(alignment.blockDistanceLeft == 0);
}

TEST_CASE("ALIGNMENT - Alternate stones with opponent stone inside") {
    Board board;

    // WBOBOBWBOB
    board.addStoneWhite({9, 4});
    board.addStoneBlack({9, 5});
    board.addStoneBlack({9, 7});
    board.addStoneBlack({9, 9});
    board.addStoneWhite({9, 10});
    board.addStoneBlack({9, 11});
    board.addStoneBlack({9, 13});

    Alignment alignment = AlignmentChecker::detectAlignment({9, 9}, board.getLineGridBlack(), board.getLineGridWhite(), {0, 1});
    REQUIRE(alignment.nbRealAlignment == 2);
    REQUIRE(alignment.nbPotentialAlignment == 3);
    REQUIRE(alignment.holeCount == 2);
    REQUIRE(alignment.blockDistanceRight == 1);
    REQUIRE(alignment.blockDistanceLeft == 0);
}

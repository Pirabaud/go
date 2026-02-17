//
// Created by pirabaud on 1/14/26.
//

#include "Board.h"
#include "HeuristicService.h"
#include "scores.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("HEURISTIC - Check win score") {

    std::array<uint64_t, 4> right {0, 0, 1, 1 };
    std::array<uint64_t, 4> left {1, 1, 0, 0};

    REQUIRE(HeuristicService::getHeuristicValue(right, left) == Scores::WIN);
}
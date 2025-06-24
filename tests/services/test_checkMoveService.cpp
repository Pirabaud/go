//
// Created by pirabaud on 6/24/25.
//

#include <catch2/catch_test_macros.hpp>
#include "services/CheckMoveService.h"
#include <array>

TEST_CASE("checkCapture fonctionne", "[gomoku]") {
    std::array<uint32_t, 19> gridBlack{};
    std::array<uint32_t, 19> gridWhite{};

    // Exemple : simulate un motif de capture
    int x = 10;
    int y = 10;
    gridBlack[x + 1] = 1u << (18 - (y + 1));        // Friendly stone
    gridWhite[x + 2] = 1u << (18 - (y + 2));        // Enemy stone
    gridWhite[x - 1] = 1u << (18 - (y - 1));        // Enemy stone

    bool result = CheckMoveService::isLegalMove(x, y, gridBlack, gridWhite, true);
    REQUIRE(result == true);
}
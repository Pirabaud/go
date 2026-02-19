#include <iostream>

#include "HeuristicService.h"
#include "../../cmake-build-debug/_deps/catch2-src/src/catch2/catch_test_macros.hpp"
#include "catch2/internal/catch_compiler_capabilities.hpp"
#include "catch2/internal/catch_test_registry.hpp"
#include "minmax/MinMax.hpp"


TEST_CASE("MinMax - Attach board reference working") {
    Board board;
    board.addStoneBlack({1, 1});

    MinMax minmax(board);
    minmax.getBoard().addStoneWhite({1, 1});

}

TEST_CASE("MinMax - Dilemma: Immediate Win vs Capture") {
    Board board;
    MinMax ai(board);
    HeuristicService::init();

    // SETUP : Noir a un alignement de 4 (O O O O _) -> Victoire en (10, 14)
    board.addStoneBlack({10, 10});
    board.addStoneBlack({10, 11});
    board.addStoneBlack({10, 12});
    board.addStoneBlack({10, 13});

    // SETUP : Une opportunité de capture tentante ailleurs
    // Noir: X, Blanc: O O. Noir peut manger en jouant X O O [X]
    board.addStoneBlack({5, 5});
    board.addStoneWhite({5, 6});
    board.addStoneWhite({5, 7});
    std::cout <<
    ai.getBoard().getScore() << std::endl;
    // Le coup de capture serait {5, 8}

    // ACTION : On demande à l'IA de jouer (Noir)
    // On met un temps court, car la victoire doit être vue immédiatement
    auto result = ai.run(500, true);
    Position bestMove = result.first;

    // VERIFICATION
    // Elle DOIT jouer le coup gagnant (10, 14), pas la capture (5, 8)
    REQUIRE(bestMove.x == 10);
    REQUIRE(bestMove.y == 14);
}
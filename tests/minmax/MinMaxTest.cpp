#include "../../cmake-build-debug/_deps/catch2-src/src/catch2/catch_test_macros.hpp"
#include "minmax/MinMax.hpp"


TEST_CASE("MinMax - Attach board reference working") {
    Board board;
    board.addStoneBlack({1, 1});

    MinMax minmax(board);
    minmax.getBoard().addStoneWhite({1, 1});

}
#include "AlignmentChecker.hpp"

#include <algorithm>
#include <iostream>

#include "CheckLegalMove.hpp"


Alignment AlignmentChecker::detectAlignment(const Position pos, const Board::StoneMask &grid,
                                            const Board::StoneMask &gridOpposite, const Position dir) {

    Alignment result = {
        .blockDistanceRight = 1,
        .blockDistanceLeft = 1,
        .nbRealAlignment = 0
    };
    const Result resultRight = countDirection(pos, dir, grid, gridOpposite);
    const Result resultLeft = countDirection(pos, {-dir.x, -dir.y}, grid, gridOpposite);

    result.nbRealAlignment = std::max(resultRight.countAfterFirstHole + resultRight.countBeforeFirstHole + resultLeft.countBeforeFirstHole, resultLeft.countAfterFirstHole + resultLeft.countBeforeFirstHole + resultRight.countBeforeFirstHole) + 1;
    result.nbPotentialAlignment = resultRight.countBeforeFirstHole + resultRight.countAfterFirstHole + resultRight.countAfterRealAlignment +
                                    resultLeft.countBeforeFirstHole + resultLeft.countAfterFirstHole + resultLeft.countAfterRealAlignment + 1;
    result.blockDistanceRight = resultRight.blockDistance;
    result.blockDistanceLeft = resultLeft.blockDistance;
    result.holeCount = resultRight.countHole + resultLeft.countHole;
    return {result};
}

AlignmentChecker::Result AlignmentChecker::countDirection(Position pos, Position dir, const Board::StoneMask &grid,
    const Board::StoneMask &gridOpposite) {
    Result result{};

    int cx = pos.x  + dir.x;
    int cy = pos.y + dir.y;
    for (int i = 1; i < 5; i++) {
        if (CheckLegalMove::notInBoard({cx, cy}) || Board::isStoneAt(gridOpposite, {cx, cy})) {
            result.blockDistance = i;
            break;
        }

        if (!Board::isStoneAt(grid, {cx, cy})) {
            result.countHole++;
        } else {
            switch (result.countHole) {
                case 0: result.countBeforeFirstHole++; break;
                case 1: result.countAfterFirstHole++; break;
                default: result.countAfterRealAlignment++; break;
            }
        }

        cx = cx  + dir.x;
        cy = cy + dir.y;
    }
    return result;
}
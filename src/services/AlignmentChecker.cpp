#include "AlignmentChecker.hpp"

#include <algorithm>
#include <bitset>
#include <iostream>

Alignment AlignmentChecker::detectAlignment(const Position pos, const Board::StoneMask &grid,
                                            const Board::StoneMask &gridOpposite, const Position dir) {

    Alignment result = {
        .block = NOTALIGN,
        .nbAlignment = 0
    };
        const Result resultRight = countDirection(pos, dir, grid, gridOpposite);
        const Result resultLeft = countDirection(pos, {-dir.x, -dir.y}, grid, gridOpposite);

         result.nbAlignment = std::max(resultRight.countAfterHole + resultRight.countBeforeHole + resultLeft.countBeforeHole, resultLeft.countAfterHole + resultLeft.countBeforeHole + resultRight.countBeforeHole) + 1;

            switch (resultRight.blocked + resultLeft.blocked) {
                case 1:
                    result.block = BlockState::SEMIBLOCKED;
                    break;
                case 2:
                    result.block = BlockState::BLOCKED;
                    break;
                default:
                    result.block = BlockState::FREE;
            }
    return result;
}

AlignmentChecker::Result AlignmentChecker::countDirection(Position pos, Position dir, const Board::StoneMask &grid,
    const Board::StoneMask &gridOpposite) {
    Result result{};
    int hole = 0;

    int cx = pos.x  + dir.x;
    int cy = pos.y + dir.y;
    while (hole < 2) {

        if (Board::isStoneAt(gridOpposite, {cx, cy}) == 1) {
            break;
        }

        if ( cx < 0 || cx >= Board::SIZE || cy < 0 || cy >= Board::SIZE) {
            break;
        }
        if (Board::isStoneAt(grid, {cx, cy}) == 0) {
            hole++;
        }

        else if (hole == 1) {
            result.countAfterHole++;
        }
        else {
            result.countBeforeHole++;
        }
        if (hole == 2) {
            break;
        }
        cx += dir.x;
        cy += dir.y;
    }
    if (cx < 0 || cx >= Board::SIZE || cy < 0 || cy >= Board::SIZE)
        result.blocked = true;
    else
        result.blocked = ((gridOpposite.at(cx) >> (Board::SIZE - 1 - cy)) & 1) != 0;
    return result;
}
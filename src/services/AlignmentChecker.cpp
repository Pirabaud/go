#include "AlignmentChecker.hpp"

#include <iostream>

Alignment AlignmentChecker::detectAlignment(const Position pos, const int count, Board::StoneMask &grid,
                                            Board::StoneMask &gridOpposite) {
    std::array directions = {
        //std::make_pair(0, 1),
        //std::make_pair(1, 0),
        std::make_pair(1, -1),
        //std::make_pair(1, 1),
    };
    for (auto& [dx, dy] : directions) {
        Result result1 = countDirection(pos, {dx, dy}, count, grid, gridOpposite);
        Result result2 = countDirection(pos, {-dx, -dy}, count, grid, gridOpposite);

        if (result1.count + result2.count + 1 >= count) {
            switch (result1.blocked + result2.blocked) {
                case 1:
                    return Alignment::SEMIBLOCKED;
                case 2:
                    return Alignment::BLOCKED;
                default:
                    return Alignment::FREE;
            }
        }
    }
    return Alignment::NOTALIGN;
}

AlignmentChecker::Result AlignmentChecker::countDirection(Position pos, Position dir, int count, Board::StoneMask &grid,
    Board::StoneMask &gridOpposite) {
    Result result{};

    int cx = pos.x  + dir.x;
    int cy = pos.y + dir.y;
    for (int i = 0; i < count; i++) {
        //for (int j = 0; j < Board::SIZE; j++) {
          //  std::cout << std::bitset<Board::SIZE>(grid.at(j)) << std::endl;
        //}

        if ( cx < 0 || cx >= Board::SIZE || cy < 0 || cy >= Board::SIZE) {
            break;
        }
        if (((grid.at(cx) >> (Board::SIZE - 1 - cy)) & 1) == 0) {
            break;
        }

        result.count++;

        cx += dir.x;
        cy += dir.y;
    }
    if (cx < 0 || cx >= Board::SIZE || cy < 0 || cy >= Board::SIZE)
        result.blocked = true;
    else
        result.blocked = ((gridOpposite.at(cx) >> (Board::SIZE - 1 - cy)) & 1) != 0;
    return result;
}

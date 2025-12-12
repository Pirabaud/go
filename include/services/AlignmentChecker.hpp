//
// Created by dev-ecomicro on 20/07/2025.
//

#ifndef ALIGNMENTCHECKER_HPP
#define ALIGNMENTCHECKER_HPP
#include "Board.h"
#include "LineBlockState.hpp"
#include "Position.hpp"

class AlignmentChecker {

public:
    static int getBlockFactor(int blockDistance);

    static bool check5Alignment(const std::array<uint64_t, 6> &bitBoard, int dir);

    static Alignment detectAlignment(Position pos, const Board::StoneMask &grid, const Board::StoneMask &gridOpposite, Position dir);

private:
    struct Result {
        int countBeforeFirstHole;
        int countAfterFirstHole;
        int countHole;
        int countAfterRealAlignment;
        int blockDistance;
    };

    static Result countDirection(Position pos, Position dir, const Board::StoneMask &grid, const Board::StoneMask &gridOpposite);

};


#endif //ALIGNMENTCHECKER_HPP

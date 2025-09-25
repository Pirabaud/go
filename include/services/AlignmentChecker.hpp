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
    static Alignment detectAlignment(Position pos, int count, const Board::StoneMask &grid,  const Board::StoneMask &gridOpposite);

private:
    struct Result {
        int count;
        bool blocked;
    };

    static Result countDirection(Position pos, Position dir, int count, const Board::StoneMask &grid, const Board::StoneMask &gridOpposite);

};


#endif //ALIGNMENTCHECKER_HPP

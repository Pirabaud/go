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

    static bool checkWinAlignment(const std::array<uint64_t, 6> &allyBitBoard, const std::array<uint64_t, 6> &enemyBitBoard, int dir);

    static Alignment checkAlignment(const std::array<uint64_t, 4> &line);
};


#endif //ALIGNMENTCHECKER_HPP

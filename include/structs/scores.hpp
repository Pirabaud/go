//
// Created by pirabaud on 1/13/26.
//

#ifndef GOMOKU_SCORES_HPP
#define GOMOKU_SCORES_HPP

// Scores.hpp
namespace Scores {
    static const int WIN        = 1000000;

    static const int FOUR_OPEN  = 100000;

    static const int FOUR       = 10000;
    static const int FOUR_ONE_HOLE  = 10000;

    static const int THREE_OPEN = 5000;
    static const int THREE_ONE_HOLE = 4000;

    static const int THREE_ONE_HOLE_BLOCKED = 5000;
    static const int FOUR_DOUBLE_HOLE = 2000;
    static const int THREE        = 1000;

    static const int TWO_OPEN     = 500;
    static const int THREE_TWO_HOLE  = 100;
    static const int TWO          = 100;
}
#endif //GOMOKU_SCORES_HPP
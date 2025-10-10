//
// Created by dev-ecomicro on 20/07/2025.
//

#ifndef POSITION_HPP
#define POSITION_HPP
#include <cstddef>

struct Position {
    int x;
    int y;
};

inline bool operator<(const Position& a, const Position& b) {
    return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

inline bool operator==(const Position& a, const Position& b) {
    return a.x == b.x && a.y == b.y;
}

#endif //POSITION_HPP

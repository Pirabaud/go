//
// Created by pirabaud on 6/19/25.
//

#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <cstdint>
#include <set>


#include "Stone.h"


class Board {
public:
    static constexpr int SIZE = 19;
    static constexpr int MAXCANDIDATESPOSITIONS = 3;
    Board();
    ~Board() = default;
    void printBit();
    [[nodiscard]] std::array<uint32_t, Board::SIZE> getGridWhite() const;
    uint32_t getGridBlack() const;

private:
    std::array<uint32_t, SIZE> gridWhite;
    uint32_t gridBlack[SIZE];
    std::set<Stone> candidatesPositionsWhite;
    std::set<Stone> candidatesPositionsBlack;

};



#endif //BOARD_H

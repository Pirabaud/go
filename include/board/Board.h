//
// Created by pirabaud on 6/19/25.
//

#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <cstdint>
#include <iosfwd>
#include <set>

class Board {
public:
    static constexpr int SIZE = 19;
    static constexpr int MAXCANDIDATESPOSITIONS = 3;

    [[nodiscard]] std::array<uint32_t, Board::SIZE>& getGridWhite();
    [[nodiscard]] std::array<uint32_t, Board::SIZE>& getGridBlack();

    void addStoneWhite(int x, int y);

    void addStoneBlack(int x, int y);


    Board();
    ~Board() = default;

private:
    std::array<uint32_t, SIZE> gridWhite{};
    std::array<uint32_t, SIZE> gridBlack{};
    std::set<uint32_t> candidatesPositionsWhite;
    std::set<uint32_t> candidatesPositionsBlack;

};

std::ostream& operator<<(std::ostream& os, Board& board);



#endif //BOARD_H

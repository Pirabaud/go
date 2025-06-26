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


    typedef std::array<uint32_t, SIZE> StoneMask;


    [[nodiscard]] const StoneMask& getGridWhite() const;
    [[nodiscard]] const StoneMask& getGridBlack() const;

    void addStoneWhite(const int &x, const int &y);

    void addStoneBlack(const int &x, const int &y);


    Board();
    ~Board() = default;

private:
    StoneMask gridWhite{};
    StoneMask gridBlack{};
    std::set<uint32_t> candidatesPositionsWhite;
    std::set<uint32_t> candidatesPositionsBlack;
};

std::ostream& operator<<(std::ostream& os, const Board& board);


#endif //BOARD_H

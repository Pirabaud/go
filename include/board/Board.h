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
    static constexpr int SIZE = 9;
    static constexpr int MAXCANDIDATESPOSITIONS = 3;
    static constexpr int FULL_ROW = 0b11111111111111111111111111111111;

    typedef std::array<uint32_t, SIZE> StoneMask;

    static bool isStoneAt(const StoneMask& grid, const int& rowIndex, const int& colIndex);
    static void removeStoneAt(StoneMask& grid, const int& rowIndex, const int& colIndex);

    [[nodiscard]] StoneMask& getGridWhite();
    [[nodiscard]] StoneMask& getGridBlack();

    void addStoneWhite(const int& rowIndex, const int& colIndex);
    void addStoneBlack(const int& rowIndex, const int& colIndex);

    void removeWhiteStoneAt(const int& rowIndex, const int& colIndex);
    void removeBlackStoneAt(const int& rowIndex, const int& colIndex);


    bool isBlackStoneAt(const int& rowIndex, const int& colIndex) const;
    bool isWhiteStoneAt(const int& rowIndex, const int& colIndex) const;


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

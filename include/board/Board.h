//
// Created by pirabaud on 6/19/25.
//

#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <cstdint>
#include <iosfwd>
#include <set>

#include "Position.hpp"

class Board {
public:
    static constexpr int SIZE = 19;
    static constexpr int MAXCANDIDATESPOSITIONS = 3;
    static constexpr int FULL_ROW = 0b11111111111111111111111111111111;

    typedef std::array<uint32_t, SIZE> StoneMask;

    static bool isStoneAt(const StoneMask& grid, Position pos);
    static void removeStoneAt(StoneMask& grid, Position pos);

    [[nodiscard]] StoneMask& getGridWhite();
    [[nodiscard]] StoneMask& getGridBlack();

    void addStoneWhite(Position pos);
    void addStoneBlack(Position pos);

    void removeWhiteStoneAt(Position pos);
    void removeBlackStoneAt(Position pos);

    void resolveCaptures();

    [[nodiscard]] bool isBlackStoneAt(Position pos) const;
    [[nodiscard]] bool isWhiteStoneAt(Position pos) const;
    void emptyColumn(int col);
    void emptyLine(int col);


    Board();

    ~Board() = default;

private:
    StoneMask gridWhite{};

    StoneMask gridBlack{};

    std::set<uint32_t> candidatesPositionsWhite;

    std::set<uint32_t> candidatesPositionsBlack;

    static void removeSToneCaptureAtPosition(StoneMask &enemyMask, Position pos, Position dir);
    bool resolveCaptureAtPosition(Position pos);
    bool resolveCaptureAtPositionInDirection(Position pos, Position dir);
};

std::ostream& operator<<(std::ostream& os, const Board& board);


#endif //BOARD_H

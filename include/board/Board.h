//
// Created by pirabaud on 6/19/25.
//

#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <cstdint>
#include <iosfwd>
#include <set>
#include <vector>

#include "Position.hpp"

class Board {
public:
    static constexpr int SIZE = 19;
    static constexpr int FULL_ROW = 0b1111111111111111111;

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

    void save();
    void restore();

    [[nodiscard]] bool isBlackStoneAt(Position pos) const;
    [[nodiscard]] bool isWhiteStoneAt(Position pos) const;
    void emptyColumn(int col);
    void emptyLine(int col);

    void printBoard() const;

    int getWhiteCaptured() const;

    int getBlackCaptured() const;


    Board();

    ~Board() = default;

private:
    StoneMask gridWhite{};
    StoneMask gridBlack{};

    std::vector<StoneMask> saveGridWhite;
    std::vector<StoneMask> saveGridBlack;

    int blackStoneCaptured = 0;
    int whiteStoneCaptured = 0;

    void removeStoneCaptureAtPosition(StoneMask &enemyMask, Position pos, Position dir);
    bool resolveCaptureAtPosition(Position pos);
    bool resolveCaptureAtPositionInDirection(Position pos, Position dir);
};

std::ostream& operator<<(std::ostream& os, const Board& board);


#endif //BOARD_H

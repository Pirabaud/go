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
    static constexpr int SIZE = 15;
    static constexpr int FULL_ROW = 0b1111111111111111111;

    typedef std::array<uint32_t, SIZE> StoneMask;
    typedef std::array<uint32_t, SIZE * 2> StoneMaskDiag;


   static std::array<uint64_t, 6> shift_right_board(const std::array<uint64_t, 6> &currentBitboard, int shift);
    static std::array<uint64_t, 6> bitBoardAnd(const std::array<uint64_t, 6> &BitboardDest, const std::array<uint64_t, 6> &currentBitSrc);

    static bool isStoneOnLineAndDiagAt(const StoneMask& grid, Position pos);
    static bool isStoneOnDiagAt(const StoneMaskDiag &grid, Position pos);
    void removeStoneAt(bool color, Position pos);

    [[nodiscard]] StoneMask& getLineGridWhite();
    [[nodiscard]] StoneMask& getLineGridBlack();
    [[nodiscard]] StoneMask& getColGridWhite();
    [[nodiscard]] StoneMask& getColGridBlack();
    [[nodiscard]] StoneMaskDiag& getDiagRightGridWhite();
    [[nodiscard]] StoneMaskDiag& getDiagRightGridBlack();
    [[nodiscard]] StoneMaskDiag& getDiagLeftGridWhite();
    [[nodiscard]] StoneMaskDiag& getDiagLeftGridBlack();

    void addStoneWhite(Position pos);
    void addStoneBlack(Position pos);

    void removeWhiteStoneAt(Position pos);

    void removeBlackStoneAt(Position pos);


    void save();
    void restore();

    [[nodiscard]] bool isBlackStoneAt(Position pos) const;
    [[nodiscard]] bool isWhiteStoneAt(Position pos) const;

    static int getGlobalIndex(Position pos) ;

    void emptyColumn(int col);
    void emptyLine(int col);

    void printBoard() const;
    static void printLineAndCol(const StoneMask &grid);
    static void printDiag(const StoneMaskDiag &grid);

    int getWhiteCaptured() const;

    int getBlackCaptured() const;

    [[nodiscard]] std::array<uint64_t, 6>& getBitBoardWhite() ;
    [[nodiscard]] std::array<uint64_t, 6>& getBitBoardBlack() ;

    static bool isBitAt(const std::array<uint64_t, 6>& bitBoard, int globalIndex) ;
    static void clearBitAt( std::array<uint64_t, 6>& bitBoard, int globalIndex);


    Board();

    ~Board() = default;

private:
    std::array<uint64_t, 6> bitBoardWhite{};
    std::array<uint64_t, 6> bitBoardBlack{};
    StoneMask gridLineWhite{};
    StoneMask gridLineBlack{};
    StoneMask gridColWhite{};
    StoneMask gridColBlack{};
    StoneMaskDiag gridDiagRightWhite{};
    StoneMaskDiag gridDiagRightBlack{};
    StoneMaskDiag gridDiagLeftWhite{};
    StoneMaskDiag gridDiagLeftBlack{};

    std::vector<StoneMask> saveGridWhite;
    std::vector<StoneMask> saveGridBlack;

    int blackStoneCaptured = 0;
    int whiteStoneCaptured = 0;

    void removeStoneCaptureAtPosition(StoneMask &enemyMask, Position pos, Position dir);
    static void fillEmptyGridLineAndCol(StoneMask &grid) ;
    static void fillEmptyGridDiag(StoneMaskDiag &grid) ;
};

std::ostream& operator<<(std::ostream& os, const Board& board);


#endif //BOARD_H

//
// Created by pirabaud on 6/19/25.
//

#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <cstdint>
#include <iosfwd>

#include "Position.hpp"

class Board {
public:
    static constexpr int SIZE = 19;

   static std::array<uint64_t, 6> shift_right_board(const std::array<uint64_t, 6> &currentBitboard, int shift);
    static std::array<uint64_t, 6> shift_left_board(const std::array<uint64_t, 6> &currentBitboard, int shift);
    static std::array<uint64_t, 6> bitBoardAnd(const std::array<uint64_t, 6> &bitBoard1, const std::array<uint64_t, 6> &bitBoard2);
    static std::array<uint64_t, 6> bitBoardOr(const std::array<uint64_t, 6> &bitBoard1, const std::array<uint64_t, 6> &bitBoard2);

    void addStoneWhite(Position pos);
    void addStoneBlack(Position pos);

    void addCaptures(bool forWhitePlayer, int stoneCount);

    bool isEmpty() const;

    static int getGlobalIndex(Position pos) ;


    [[nodiscard]] int getWhiteCaptured() const;

    [[nodiscard]] int getBlackCaptured() const;

    [[nodiscard]] std::array<uint64_t, 6> &getBitBoardWhite();
    [[nodiscard]] std::array<uint64_t, 6> &getBitBoardBlack();
    [[nodiscard]] const std::array<uint64_t, 6> &getBitBoardWhite() const;
    [[nodiscard]] const std::array<uint64_t, 6> &getBitBoardBlack() const;

    static bool isBitAt(const std::array<uint64_t, 6>& bitBoard, int globalIndex) ;
    static void clearBitAt( std::array<uint64_t, 6>& bitBoard, int globalIndex);

    ~Board() = default;

private:
    std::array<uint64_t, 6> bitBoardWhite{};
    std::array<uint64_t, 6> bitBoardBlack{};

    int blackStoneCaptured = 0;
    int whiteStoneCaptured = 0;

};

std::ostream& operator<<(std::ostream& os, const Board& board);


#endif //BOARD_H

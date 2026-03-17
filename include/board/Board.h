#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <iosfwd>
#include "Position.hpp"

class Board {
public:
    static uint64_t ZOBRIST_TABLE[400][2];

    static int SIZE;

    static std::array<uint64_t, 6> shift_right_board(const std::array<uint64_t, 6> &currentBitboard, int shift);
    static std::array<uint64_t, 6> shift_left_board(const std::array<uint64_t, 6> &currentBitboard, int shift);
    static std::array<uint64_t, 6> bitBoardAnd(const std::array<uint64_t, 6> &bitBoard1, const std::array<uint64_t, 6> &bitBoard2);
    static std::array<uint64_t, 6> bitBoardOr(const std::array<uint64_t, 6> &bitBoard1, const std::array<uint64_t, 6> &bitBoard2);

    void addStoneWhite(Position pos);
    void addStoneBlack(Position pos);
    void addStoneWhite(int index);
    void addStoneBlack(int index);
    void removeWhiteStone(int index);
    void removeBlackStone(int index);

    int getPatternIndex(int positionIndex, bool isBlackPlayer, int direction) const;


    void addCaptures(bool isStoneWhite, int stoneCount);
    void removeCaptures(bool isStoneWhite, int stoneCount);

    bool isEmpty() const;

    static int getGlobalIndex(Position pos) ;


    [[nodiscard]] int getWhiteCaptured() const;

    [[nodiscard]] int getBlackCaptured() const;

    [[nodiscard]] std::array<uint64_t, 6> &getBitBoardWhite();
    [[nodiscard]] std::array<uint64_t, 6> &getBitBoardBlack();
    [[nodiscard]] const std::array<uint64_t, 6> &getBitBoardWhite() const;
    [[nodiscard]] const std::array<uint64_t, 6> &getBitBoardBlack() const;

    static void setBoardSize(int size);

    static bool isBitAt(const std::array<uint64_t, 6>& bitBoard, int globalIndex) ;
    static void clearBitAt( std::array<uint64_t, 6>& bitBoard, int globalIndex);

    static void initZobrist();

    void updateCurrentZobristKey(int index, bool isBlack);

    ~Board() = default;

    uint64_t currentZobristKey = 0;


private:
    std::array<uint64_t, 6> bitBoardWhite{};
    std::array<uint64_t, 6> bitBoardBlack{};

    int blackStoneCaptured = 0;
    int whiteStoneCaptured = 0;

};

std::ostream& operator<<(std::ostream& os, const Board& board);


#endif //BOARD_H

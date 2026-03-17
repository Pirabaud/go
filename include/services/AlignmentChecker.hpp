#ifndef ALIGNMENTCHECKER_HPP
#define ALIGNMENTCHECKER_HPP
#include "Board.hpp"

class AlignmentChecker {
public:
    static bool checkWinAlignment(Board& board, bool isBlack, int dir);
    static std::array<int, 15> checkBreakableAlignment(const std::array<uint64_t, 6>& allyBitBoard,
                                                       const std::array<uint64_t, 6>& enemyBitBoard, int dir);
    static bool checkWinAt(const std::array<uint64_t, 6>& allyBB, int index);

private:
    static int countLines(const std::array<uint64_t, 6>& allyBitBoard, int index, int dir);
};


#endif //ALIGNMENTCHECKER_HPP

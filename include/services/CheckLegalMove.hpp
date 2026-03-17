#ifndef CHECK_MOVE_SERVICE_H
#define CHECK_MOVE_SERVICE_H

#include "Board.hpp"
#include "IllegalMoves.hpp"


class CheckLegalMove {
public:
    static IllegalMoves::Type isLegalMove(
        int posIndex, Board& board, const bool& isBlack);
    static bool createsAutoCapture(int posIndex, Board& board, const bool& isBlack);
    static bool checkCaptureInDirection(Board& board, int globalIndex, int dir, bool isBlack);
    static bool createsDoubleFreeThree(int posIndex, Board& board, const bool& isBlack);
};


#endif //CHECK_MOVE_SERVICE_H

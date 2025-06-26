#ifndef CHECK_MOVE_SERVICE_H
#define CHECK_MOVE_SERVICE_H

#include "Board.h"
#include "IllegalMoves.hpp"


class CheckMoveService {
public:
    static IllegalMoves::Type isLegalMove(const int& x, const int& y,
                                          const std::array<uint32_t, Board::SIZE>& gridBlack,
                                          const std::array<uint32_t, Board::SIZE>& gridWhite, const bool& isBlack);

private:
    static bool notInBoard(const int& x, const int& y);
    static bool alreadyStone(const int& x, const int& y, const std::array<uint32_t, Board::SIZE>& gridBlack,
                             const std::array<uint32_t, Board::SIZE>& gridWhite);
    static bool checkDirectionCreatingCapture(const int& x, const int& y,
                                              const std::array<uint32_t, Board::SIZE>& gridColor,
                                              const std::array<uint32_t, Board::SIZE>& gridOpposite);
    static bool checkCapture(const int& x, const int& y, const std::array<uint32_t, Board::SIZE>& gridColor,
                             const std::array<uint32_t, Board::SIZE>& gridOpposite, const int& dx, const int& dy);
    CheckMoveService() = default;
};


#endif //CHECK_MOVE_SERVICE_H

#ifndef CHECK_MOVE_SERVICE_H
#define CHECK_MOVE_SERVICE_H

#include "Board.h"
#include "IllegalMoves.hpp"
#include "Position.hpp"
#include "AlignmentChecker.hpp"


class CheckMoveService {
public:
    static IllegalMoves::Type isLegalMove(
        Position pos, Board &board, const bool &isBlack);
    static bool notInBoard(Position pos);
    static bool checkDirectionAlignStone(const int &x, const int &y, const int &count, std::array<uint32_t, Board::SIZE>&grid);

private:

    static bool checkAlignStone(const int &x, const int &y, const int &dx, const int &dy, const int &count, std::array<uint32_t, Board::SIZE>&grid);

    static bool alreadyStone(Position pos,
                             const std::array<unsigned, 19> &gridBlack, Board::StoneMask &gridWhite);

    static bool checkDirectionCreatingCapture(
        Position pos,
        const std::array<unsigned, 19> &gridColor, const std::array<unsigned, 19> &gridOpposite);

    static bool checkCapture(Position pos,
                             const std::array<unsigned, 19> &gridColor, const std::array<unsigned, 19> &gridOpposite, Position direction);

    static bool checkDoubleThree(Position pos, Board::StoneMask grid, Board::StoneMask gridOpposite);


    CheckMoveService() = default;
};


#endif //CHECK_MOVE_SERVICE_H

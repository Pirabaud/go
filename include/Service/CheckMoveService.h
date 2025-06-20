//
// Created by pirabaud on 6/20/25.
//

#ifndef CHECKMOVESERVICE_H
#define CHECKMOVESERVICE_H

#include "Board.h"


class CheckMoveService {

public:
    static bool isLegalMove(const int &x, const int &y, const std::array<uint32_t, Board::SIZE>&,
        const std::array<uint32_t, Board::SIZE>&, const bool &);

private:
    static bool NotInBoard(const int &x, const int &y);
    static bool alreadyStone(const int &x, const int &y, const std::array<uint32_t, Board::SIZE> &,
    const std::array<uint32_t, Board::SIZE> &);
    static bool checkSelfCapture(const int &dx, const int &dy, const std::array<uint32_t, Board::SIZE> &,
    const std::array<uint32_t, Board::SIZE> &, uint32_t &, const bool &);
    static bool canSelfCapture(const int &dx, const int &dy, const std::array<uint32_t, Board::SIZE> &,
    const std::array<uint32_t, Board::SIZE> &, const bool &);
    static bool checkLineCapture(const int &x, const int &y, const std::array<uint32_t, Board::SIZE> &,
        const std::array<uint32_t, Board::SIZE> &, const bool &);
    CheckMoveService() = default;
};



#endif //CHECKMOVESERVICE_H

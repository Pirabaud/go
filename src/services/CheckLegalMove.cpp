#include "../../include/services/CheckLegalMove.hpp"

#include <algorithm>
#include <bitset>
#include <iostream>


#include "CaptureService.hpp"
#include "FreeThreeService.h"


IllegalMoves::Type CheckLegalMove::isLegalMove(Position pos,
                                               Board &board,
                                               const bool &isBlack) {
    return IllegalMoves::Type::NONE;
}






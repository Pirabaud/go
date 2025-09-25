#ifndef CAPTURE_SERVICE_HPP
#define CAPTURE_SERVICE_HPP
#include "Board.h"
#include "Position.hpp"


class CaptureService {
public:
    static bool resolveCaptureAtPosition(Board &board, Position pos, bool isBlack);

private:
    static bool resolveCaptureAtPositionInDirection(Board &board, Position pos, Position dir, bool color);
};


#endif //CAPTURE_SERVICE_HPP

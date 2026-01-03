#ifndef CAPTURE_SERVICE_HPP
#define CAPTURE_SERVICE_HPP
#include "Board.h"
#include "Direction.hpp"
#include "Position.hpp"


class CaptureService {
public:
    static int checkCapture(const std::array<uint64_t, 6> &allyBitBoard, std::array<uint64_t, 6> &enemyBitBoard, Position pos);
    static bool winLineBreakable(const std::array<uint64_t, 6> &allyBitBoard, const std::array<uint64_t, 6> &enemyBitBoard, int startIndex, int dirAlignment);
private:
    static int checkCaptureInDirection(const std::array<uint64_t, 6> &allyBitBoard, std::array<uint64_t, 6> &enemyBitBoard, int globalIndex, int dir);

};


#endif //CAPTURE_SERVICE_HPP

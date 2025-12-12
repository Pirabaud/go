#ifndef CAPTURE_SERVICE_HPP
#define CAPTURE_SERVICE_HPP
#include "Board.h"
#include "Position.hpp"


class CaptureService {
public:
    static bool checkCapture(const std::array<uint64_t, 6> &allyBitBoard, std::array<uint64_t, 6> &enemyBitBoard, Position pos);
private:
    static bool checkCaptureInDirection(const std::array<uint64_t, 6> &allyBitBoard, std::array<uint64_t, 6> &enemyBitBoard, int globalIndex, int dir);

};


#endif //CAPTURE_SERVICE_HPP

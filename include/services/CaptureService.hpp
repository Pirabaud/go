#ifndef CAPTURE_SERVICE_HPP
#define CAPTURE_SERVICE_HPP
#include "Board.h"
#include "Direction.hpp"
#include "Position.hpp"


class CaptureService {
public:
    static int checkCapture(Board &board, int globalIndex, bool isBlack, int *capture, int &count);
    static bool winLineBreakable(const std::array<uint64_t, 6> &allyBitBoard, const std::array<uint64_t, 6> &enemyBitBoard, int startIndex, int dirAlignment);
private:
    static int checkCaptureInDirection(Board& board, const int globalIndex,
                                            const int dir, const bool isBlack, int* capture, int &count);

};


#endif //CAPTURE_SERVICE_HPP

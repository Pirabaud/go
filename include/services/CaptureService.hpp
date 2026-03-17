#ifndef CAPTURE_SERVICE_HPP
#define CAPTURE_SERVICE_HPP
#include "Board.hpp"
#include "SFML/Audio.hpp"


class CaptureService {
public:
    static int checkCapture(Board& board, int globalIndex, bool isBlack, int* capture, int& count, bool removeStone = true);
    static bool winLineBreakable(
        Board& board, bool isBlack, int startIndex, int dirAlignment);
    static std::array<int, 15> getBlockingCaptureIndex(const std::array<uint64_t, 6>& allyBitBoard,
                                                       const std::array<uint64_t, 6>& enemyBitBoard, int startIndex,
                                                       int dirAlignment);

private:
    static int checkCaptureInDirection(Board& board, int globalIndex,
                                       int dir, bool isBlack, int* capture, int& count, bool removeSTone);
};


#endif //CAPTURE_SERVICE_HPP

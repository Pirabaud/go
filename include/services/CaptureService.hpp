#ifndef CAPTURE_SERVICE_HPP
#define CAPTURE_SERVICE_HPP
#include "Board.h"


class CaptureService {
public:
    static void resolveCaptures(Board& board);

private:
    static void resolveCaptureAtPosition(Board& board, const int& row, const int& col);
    static void resolveCaptureAtPositionInDirection(Board& board, const int& row, const int& col, const int& dx,
                                                    const int& dy);
};


#endif //CAPTURE_SERVICE_HPP

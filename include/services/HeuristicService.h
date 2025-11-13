#ifndef GOMOKU_HEURISTIC_SERVICE_H
#define GOMOKU_HEURISTIC_SERVICE_H
#include "Board.h"

#define WIN_WEIGHT 1000000
#define CAPTURE_WEIGHT 8000
#define CAPTURE_THREAT_WEIGHT 2000
#define FOUR_OPEN_WEIGHT 5000
#define THREE_OPEN_WEIGHT 1000
#define TWO_OPEN_WEIGHT 100

class HeuristicService {
public:
    HeuristicService() = delete;

    static int getHeuristicValue(Board& board);
    static int evaluateStonePatterns(Board& board, Position pos, bool isWhite);
    static int evaluateImmediateCaptureThreats(Board& board);
    static int evaluatePlayerCaptureThreats(Board& board, bool isWhite);
    static int evaluateCapturePatterns(Board& board, Position emptyPos, bool isWhite);
    static bool isCapturePattern(Board& board, Position pos, Position dir,
                                       const Board::StoneMask& allyMask,
                                       const Board::StoneMask& enemyMask);

    static int getHeuristicFromPos(Board &board, Position pos, bool isBlack);

    static int countAlignmentHeuristic(Board &board, const Position pos, const Board::StoneMask &grid, const Board::StoneMask &oppositeGrid);

};


#endif //GOMOKU_HEURISTIC_SERVICE_H
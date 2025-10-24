//
// Created by pirabaud on 9/25/25.
//

#ifndef GOMOKU_HEURISTIC_SERVICE_H
#define GOMOKU_HEURISTIC_SERVICE_H
#include "Board.h"


class HeuristicService {
public:
    HeuristicService() = delete;

    static int getHeuristicValue(Board& board);
    static int evaluateLines(Board& board);
    static int evaluateLine(Board& board, int starX, int startY, int dx, int dy);
};


#endif //GOMOKU_HEURISTIC_SERVICE_H
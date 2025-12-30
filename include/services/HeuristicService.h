#ifndef GOMOKU_HEURISTIC_SERVICE_H
#define GOMOKU_HEURISTIC_SERVICE_H
#include "Board.h"

class HeuristicService {
public:
    HeuristicService() = delete;

    static int getHeuristicValue(Board& board);
};


#endif //GOMOKU_HEURISTIC_SERVICE_H
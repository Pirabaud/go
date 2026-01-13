#ifndef GOMOKU_HEURISTIC_SERVICE_H
#define GOMOKU_HEURISTIC_SERVICE_H
#include "Board.h"

class HeuristicService {
public:
    HeuristicService() = delete;

    static int getHeuristicValue(std::array<uint64_t, 4> left, const std::array<uint64_t, 4> &right);
    static std::array<uint64_t, 262144> getHeuristicValues();
};


#endif //GOMOKU_HEURISTIC_SERVICE_H
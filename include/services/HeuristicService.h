#ifndef GOMOKU_HEURISTIC_SERVICE_H
#define GOMOKU_HEURISTIC_SERVICE_H
#include <map>

#include "Board.h"

#define PATTERNS_FILE_PATH "../patterns.txt"

class HeuristicService {
public:
    static constexpr int ALLY_BITS_MASK = 0b01;
    static constexpr int ENEMY_BITS_MASK = 0b10;
    static constexpr int MAX_INDEX = 262144; // 2^(2*9) = 262144 (for patterns of length 9)

    static std::array<int16_t, MAX_INDEX> PRECOMPUTED_SCORES;
    HeuristicService() = delete;

    static int getScore(const int index) {
        return PRECOMPUTED_SCORES[index];
    }
    static void init();

    static int evaluatePosition(const Board &board, int positionIndex, bool isBlackPlayer);
private:
    static std::vector<std::pair<std::string, int16_t>> loadPatternsFromFile();
    static std::string indexToString(int index);
};


#endif //GOMOKU_HEURISTIC_SERVICE_H
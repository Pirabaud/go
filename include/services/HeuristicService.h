#ifndef GOMOKU_HEURISTIC_SERVICE_H
#define GOMOKU_HEURISTIC_SERVICE_H
#include <map>

#include "Board.h"

#define PATTERNS_FILE_PATH "../patterns.txt"
#define POSSIBLE_PATTERNS_CHAR ["A", "E", "O"]
// 43690 is the greatest index that exists (EEEEEEEE -> 0b1010101010101010 -> 43690)
#define GREATEST_INDEX_POSSIBLE 0b1010101010101010 // 43690
#define ENEMY_BITS_MASK 0b10
#define ALLY_BITS_MASK 0b01

class HeuristicService {
public:
    HeuristicService() = delete;

    static int getHeuristicValue(uint16_t index);

    static std::optional<std::pair<std::string, int>> parseLine(std::string line);

    static std::map<std::string, int> getPatternsMap();

    static void generateAllStrings(unsigned long n, std::string current, std::vector<std::string> &results);

    static std::vector<std::string> generateAllLeftRightPatternsPossible(const std::string &leftRawPattern, const std::string &rightRawPattern);

    static std::map<std::string, int> getLeftRightPatternsMap();

    static std::vector<std::string> parseFileLines();
};


#endif //GOMOKU_HEURISTIC_SERVICE_H
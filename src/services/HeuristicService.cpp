#include "HeuristicService.h"

#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <regex>

#include "AlignmentChecker.hpp"
#include "scores.hpp"

std::array<int16_t, HeuristicService::MAX_INDEX> HeuristicService::PRECOMPUTED_SCORES = {};

void HeuristicService::init() {
    std::cout << "Initializing Heuristic LUT..." << std::endl;
    auto patterns = loadPatternsFromFile();
    // For each possible combination of 9 cells, compute its score
    for (int i = 0; i < MAX_INDEX; ++i) {
        std::string boardSlice = indexToString(i);
        int16_t currentMaxScore = 0;
        // For each pattern, check if it exists in the current board slice
        for (const auto& [patternStr, score] : patterns) {

            // If pattern found in board slice, add its score
            if (boardSlice.find(patternStr) != std::string::npos) {
                // Accumulate the maximum absolute score, this way, we prioritize the most significant patterns
                if (std::abs(score) > std::abs(currentMaxScore)) {
                    currentMaxScore = score;
                }
            }
        }
        PRECOMPUTED_SCORES[i] = currentMaxScore;
    }
    std::cout << "Heuristic LUT initialized." << std::endl;
}

std::vector<std::pair<std::string, int16_t>> HeuristicService::loadPatternsFromFile() {
    std::vector<std::pair<std::string, int16_t>> patterns;
    std::ifstream file(PATTERNS_FILE_PATH);

    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open " << PATTERNS_FILE_PATH << std::endl;
        return patterns;
    }

    std::string line;
    const std::regex rgx("^([AEO]{0,9}):(-?\\d+)$");
    std::smatch matches;

    while (std::getline(file, line)) {
        if (line.starts_with('#') || line.empty()) continue;

        if (std::regex_match(line, matches, rgx)) {
            const int value = std::stoi(matches[2].str());
            if (value <= -3000 || value >= 3000) {
                std::cerr << "[WARNING] Ignoring pattern with extreme value: " << line << std::endl;
                continue;
            }
            patterns.emplace_back(matches[1].str(), static_cast<int16_t>(value));
        } else {
            std::cerr << "[WARNING] Could not parse line: " << line << std::endl;
        }
    }
    return patterns;
}

std::string HeuristicService::indexToString(int index) {
    std::string patternString;
    patternString.reserve(9);

    // For each char of the string, extract 2 bits from index and map to 'A', 'E', 'O'
    for (int i = 0; i < 9; ++i) {
        const int shift = (8 - i) * 2;
        const int val = (index >> shift) & 0b11;

        if (val == ALLY_BITS_MASK) patternString += 'A';        // Ally
        else if (val == ENEMY_BITS_MASK) patternString += 'E';  // Enemy
        else patternString += 'O';                              // Empty
    }
    return patternString;
}

int HeuristicService::evaluatePosition(const Board &board, const int positionIndex, const bool isBlackPlayer) {
    int score = 0;

    //Compute pattern indices for all four directions
    // Horizontal (shift = 1)
    int patternIndex = board.getPatternIndex(positionIndex, isBlackPlayer, 1);
    score += getScore(patternIndex);
    // Vertical (shift = SIZE + 1 = 20)
    patternIndex = board.getPatternIndex(positionIndex, isBlackPlayer, Board::SIZE + 1);
    score += getScore(patternIndex);
    // Diagonal down-right (shift = SIZE + 1 + 1 = 21)
    patternIndex = board.getPatternIndex(positionIndex, isBlackPlayer, Board::SIZE + 2);
    score += getScore(patternIndex);
    // Diagonal down-left (shift = SIZE + 1 - 1 = 19)
    patternIndex = board.getPatternIndex(positionIndex, isBlackPlayer, Board::SIZE);
    score += getScore(patternIndex);
    return score;
}

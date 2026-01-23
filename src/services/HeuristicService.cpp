#include "HeuristicService.h"

#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <regex>

#include "AlignmentChecker.hpp"
#include "scores.hpp"


std::optional<std::pair<std::string, int>> HeuristicService::parseLine(std::string line) {
    const std::regex rgx("^([AEO]{0,6}):(-?\\d+)$");
    std::smatch matches;

    if (!std::regex_match(line, matches, rgx)) {
        return {};
    }
    return std::make_pair(matches[1].str(), std::stoi(matches[2].str()));
}

// Parse file and return lines as a vector of strings
std::vector<std::string> HeuristicService::parseFileLines() {
    std::vector<std::string> lines;
    // TODO voir pour changer le path du fichier avec 'pattern.txt', mais pour ça il faut le copier dans le dossier build
    std::ifstream file(PATTERNS_FILE_PATH);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << PATTERNS_FILE_PATH << std::endl;
        return lines;
    }

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

// Populate heuristicPatternsMap based on file
std::map<std::string, int> HeuristicService::getPatternsMap() {
    std::map<std::string, int> patternsMap;
    auto lines = parseFileLines();
    for (const auto& line : lines) {
        if (line.starts_with('#') || line.empty()) {
            continue; // Skip comment and empty lines
        }
        auto keyValuePair = parseLine(line);
        if (keyValuePair) {
            patternsMap[keyValuePair->first] = keyValuePair->second;
        } else {
        }
    }
    return patternsMap;
}

void HeuristicService::generateAllStrings(unsigned long n, std::string current, std::vector<std::string>& results) {
    if (current.length() == n) {
        results.push_back(current);
        return;
    }
    for (char c : {'A', 'E', 'O'}) {
        generateAllStrings(n, current + c, results);
    }
}

std::vector<std::string> HeuristicService::generateAllLeftRightPatternsPossible(const std::string& leftRawPattern, const std::string& rightRawPattern) {
    const std::vector possibleChars{'A', 'E', 'O'};
    std::vector<std::string> allSidePatternsPossible;
    std::vector<std::string> allLeftPrefixesPossible;
    std::vector<std::string> allRightSuffixesPossible;

    const auto leftPatternLength = leftRawPattern.length();
    const auto charsToGenerateForLeftPattern = 4 - leftPatternLength;
    generateAllStrings(charsToGenerateForLeftPattern, "", allLeftPrefixesPossible);

    const auto rightPatternLength = rightRawPattern.length();
    const auto charsToGenerateForRightPattern = 4 - rightPatternLength;
    generateAllStrings(charsToGenerateForRightPattern, "", allRightSuffixesPossible);

    for (const auto& leftPrefix : allLeftPrefixesPossible) {
        for (const auto& rightSuffix : allRightSuffixesPossible) {
            std::string fullPattern = leftPrefix + leftRawPattern + rightRawPattern + rightSuffix;
            allSidePatternsPossible.push_back(fullPattern);
        }
    }

    return allSidePatternsPossible;
}

std::map<std::string, int> HeuristicService::getLeftRightPatternsMap() {
    std::map<std::string, int> leftRightPatternsMap;
    std::map<std::string, int> patternsMap = getPatternsMap();

    for (const auto& [pattern, value] : patternsMap) {
        const auto patternLength = pattern.length();
        for (int i = 0; i < patternLength; ++i) {
            // If the actual character is not A (Ally), then it can't be possible
            if (pattern[i] != 'A') {
                continue;
            }
            std::string leftRawPattern = pattern.substr(0, i);
            std::string rightRawPattern = pattern.substr(i + 1);
            if (rightRawPattern.length() > 4) {
                rightRawPattern.erase(4, std::string::npos); // Ensure rightRawPattern is at most 4 characters long
            }


            auto allLeftPatternsPossible = generateAllLeftRightPatternsPossible(leftRawPattern, rightRawPattern);
            for (const auto& leftRightPattern : allLeftPatternsPossible) {
                leftRightPatternsMap[leftRightPattern] = value;
            }
        }
    }

    return leftRightPatternsMap;
}

uint16_t computeIndexFromPattern(const std::string& pattern) {
    uint16_t index = 0;
    for (size_t i = 0; i < pattern.length(); ++i) {
        if (pattern[i] == 'E') {
            index |= ENEMY_BITS_MASK;
        } else if (pattern[i] == 'A') {
            index |= ALLY_BITS_MASK;
        }
        if (i < pattern.length() - 1) {
            index <<= 2;
        }
    }
    return index;
}

int HeuristicService::getHeuristicValue(uint16_t index) {
    static std::array<int, GREATEST_INDEX_POSSIBLE> heuristicValues = {};
    static bool functionHasBeenCalled = false;
    // Generate all possible combinations of left and right alignments only on first call
    if (!functionHasBeenCalled) {
        heuristicValues.fill(0);
        functionHasBeenCalled = true;
        const std::map<std::string, int> leftRightPatternsMap = getLeftRightPatternsMap();
        for (const auto& [pattern, value] : leftRightPatternsMap) {
            uint16_t index = computeIndexFromPattern(pattern);
            heuristicValues[index] = value;
        }
        return 0;
    }
    return heuristicValues[index]; // Return the array of heuristic values
}

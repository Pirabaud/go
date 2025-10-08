//
// Created by Pierre Rabaud on 06/10/2025.
//

#include "../../include/services/AIService.hpp"

#include <iostream>
#include <list>
#include <random>

#include "CheckLegalMove.hpp"
#include "CheckWinService.hpp"

int randomBetween(const int min, const int max) {
    // Générateur aléatoire basé sur un vrai random device
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister
    std::uniform_int_distribution<> dist(min, max); // distribution uniforme
    return dist(gen);
}

std::vector<Position> newPos(Board& board, bool color) {
    std::vector<Position> result;
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 0; y < Board::SIZE; y++) {
            if (CheckLegalMove::isLegalMove({x, y}, board, color) != IllegalMoves::NONE) {
                continue;
            }
            result.push_back({x, y});
        }
    }
    return result;
}

int negamax(Board& board, int depth, bool color) {
    if (CheckWinService::isWin(board)) {
        return 10000;
    }
    if (depth == 0) {
        return 0;
    }
    int value = -std::numeric_limits<int>::max();
    const std::vector<Position> positions = newPos(board, color);
    if (positions.empty()) {
        return 0;
    }
    for (const auto position : positions) {
        color ? board.addStoneWhite(position) : board.addStoneBlack(position);
        std::cout << position.x << " " << position.y << std::endl;
        value = std::max(value, -negamax(board, depth - 1, !color));
        color ? board.removeWhiteStoneAt(position) : board.removeBlackStoneAt(position);
    }
    return value;
}

Position bestMove(Board& board, bool color) {
    const std::vector<Position> positions = newPos(board, color);
    Position bestMove = {-1, -1};
    int bestEvaluation = -10000;
    int  value = -10000;

    for (const auto position : positions) {
        color ? board.addStoneWhite(position) : board.addStoneBlack(position);

        value = std::max(value, -negamax(board, 3, !color));
        color ? board.removeWhiteStoneAt(position) : board.removeBlackStoneAt(position);
        if (value > bestEvaluation) {
            bestMove = position;
            bestEvaluation = value;
        }
        std::cout << position.x << " " << position.y << std::endl;
    }
    return bestMove;
}

AiPlay AIService::AIPlay(Board board) {
    const clock_t start = clock();
    Position pos = bestMove(board, false);
    const clock_t end = clock();
    const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    return {pos, time};
}

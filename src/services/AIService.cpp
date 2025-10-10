//
// Created by Pierre Rabaud on 06/10/2025.
//

#include "../../include/services/AIService.hpp"


#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <list>
#include <random>

#include "CheckLegalMove.hpp"
#include "CheckWinService.hpp"
#include "../../cmake-build-debug/_deps/catch2-src/src/catch2/internal/catch_stdstreams.hpp"

int randomBetween(const int min, const int max) {
    // Générateur aléatoire basé sur un vrai random device
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister
    std::uniform_int_distribution<> dist(min, max); // distribution uniforme
    return dist(gen);
}

std::vector<Position> posFromDir(Position pos, Position dir, Board board) {
    std::vector<Position> result;

    for (int i = 0; i < 2; i++) {
        pos = {pos.x + dir.x, pos.y + dir.y * i};
        if (CheckLegalMove::notInBoard(pos) || CheckLegalMove::alreadyStone(pos, board.getGridBlack(), board.getGridWhite())) {
            continue;
        }
        result.push_back(pos);
    }
    return result;
}

std::vector<Position> posFromPos(Position pos, Board board) {
    std::vector<Position> result;
    std::vector<Position> positions;
    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto [dx, dy] : directions) {
        positions = posFromDir(pos, {dx, dy}, board);
        result.insert(result.end(), positions.begin(), positions.end());
        positions = posFromDir(pos, {-dx, -dy}, board);
        result.insert(result.end(), positions.begin(), positions.end());
    }
    return result;
}



std::vector<Position> newPos(Board& board, bool color) {
    std::vector<Position> result;
    std::vector<Position> newPositions;
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 0; y < Board::SIZE; y++) {
            if (CheckLegalMove::alreadyStone({x,y}, board.getGridBlack(), board.getGridWhite())) {
                newPositions = posFromPos({x, y}, board);
            }
        }
        result.insert(result.end(), newPositions.begin(), newPositions.end());
        std::sort(result.begin(), result.end());
        result.erase(std::ranges::unique(result).begin(), result.end());
    }
    return result;
}

int evaluation(Board& board, Position pos, bool color) {
    Board::StoneMask grid = color == 1 ? board.getGridWhite() : board.getGridBlack();
    Board::StoneMask gridOpposite = color == 1 ? board.getGridBlack() : board.getGridWhite();
    int result = 0;
    AlignmentChecker::Result countAlignment1;
    AlignmentChecker::Result countAlignment2;
    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto [dx, dy] : directions) {
        countAlignment1 = AlignmentChecker::countDirection(pos, {dx, dy}, 5, grid, gridOpposite);
        countAlignment2 = AlignmentChecker::countDirection(pos, {-dx, -dy}, 5, grid, gridOpposite);

        switch (countAlignment1.blocked + countAlignment2.blocked) {
                case 1:
                    result += countAlignment1.count * 20;
                    result += countAlignment2.count * 20;
                case 2:
                    result += countAlignment1.count * 10;
                    result += countAlignment2.count * 10;
                default:
                    result += countAlignment1.count * 40;
                    result += countAlignment2.count * 40;
            }
        }
    return result;
    }

int negamax(Position pos, Board& board, int alpha, int beta, int depth, int color) {
    if (CheckWinService::isWin(board)) {
        return 10000 * color;
    }
    if (depth == 0) {
        return color * evaluation(board, pos, color);
    }
    int value = -std::numeric_limits<int>::max();
    const std::vector<Position> positions = newPos(board, color);
    if (positions.empty()) {
        return 0;
    }
   for (const auto position : positions) {
        color ? board.addStoneWhite(position) : board.addStoneBlack(position);
        value = std::max(value, -negamax(position, board, -beta, -alpha, depth - 1, -color));
        color ? board.removeWhiteStoneAt(position) : board.removeBlackStoneAt(position);
        if (value >= beta) {
            return value;
        }
        alpha = std::max(alpha, value);
    }
    return value;
}

Position bestMove(Board& board, bool color) {
    const std::vector<Position> positions = newPos(board, color);
    Position bestMove = {-1, -1};
    int bestEvaluation = -10000;
    int  value = -10000;
    int alpha = -10000;
    int beta = 10000;
    for (const auto position : positions) {
        color ? board.addStoneWhite(position) : board.addStoneBlack(position);
        value = std::max(value, -negamax(position, board, alpha, beta, 10, 1));
        color ? board.removeWhiteStoneAt(position) : board.removeBlackStoneAt(position);
        if (value > bestEvaluation) {
            bestMove = position;
            bestEvaluation = value;
        }
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

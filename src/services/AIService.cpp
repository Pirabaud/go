//
// Created by Pierre Rabaud on 06/10/2025.
//

#include "../../include/services/AIService.hpp"


#include <algorithm>
#include <fstream>
#include <unordered_set>
#include <iostream>
#include <list>
#include <random>
#include <nlohmann/json.hpp>

#include "EvaluationService.hpp"
#include "JsonService.hpp"
using nlohmann::json;

#include "CheckLegalMove.hpp"
#include "CheckWinService.hpp"
#include "../../cmake-build-debug/_deps/catch2-src/src/catch2/internal/catch_stdstreams.hpp"

std::vector<Position> posFromDir(const Position pos, const Position dir, Board& board) {
    std::vector<Position> result;
    Position temp = {};

    for (int i = 1; i < 3; i++) {
        temp = {pos.x + dir.x * i, pos.y + dir.y * i};
        if (CheckLegalMove::notInBoard(temp) || CheckLegalMove::alreadyStone(temp, board.getGridBlack(), board.getGridWhite())) {
            continue;
        }
        result.push_back(temp);
    }
    return result;
}

std::vector<Position> posFromPos(Position pos, Board& board) {
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

std::vector<Position> newPos(Board& board) {
    std::vector<Position> result;
    std::vector<Position> newPositions;
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 0; y < Board::SIZE; y++) {
            if (CheckLegalMove::alreadyStone({x,y}, board.getGridBlack(), board.getGridWhite())) {
                continue;;
                //newPositions = posFromPos({x, y}, board);
            }
            result.push_back({x,y});
            //result.insert(result.end(), newPositions.begin(), newPositions.end());
        }
    }
    //std::sort(result.begin(), result.end());
    //result.erase(std::ranges::unique(result).begin(), result.end());
    return result;
}

// Hypothèses : Board::SIZE == 3, et ces helpers existent :
// bool isWhiteStoneAt({x, y}); bool isBlackStoneAt({x, y});

namespace {
    int evalTriplet(const Board& b, bool color,
                    std::array<std::pair<int,int>,3> cells) {
        int mine = 0, opp = 0;
        for (auto [x,y] : cells) {
            bool w = b.isWhiteStoneAt({x,y});
            bool bl = b.isBlackStoneAt({x,y});
            if (w && bl) return 0;            // Cas impossible normalement, sécurité
            if (w) (color ? mine : opp)++;
            if (bl) (color ? opp  : mine)++;
        }

        if (mine > 0 && opp > 0) return 0;    // Ligne bloquée

        if (mine == 3) return +100;
        if (opp  == 3) return -100;
        if (mine == 2) return +10;
        if (opp  == 2) return -10;
        // Optionnel : récompenser un pion isolé non bloqué
        // if (mine == 1) return +1;
        // if (opp  == 1) return -1;
        return 0;
    }
}

int evaluation(const Board& b, bool color) {
    static int count = 0;
    int score = 0;

    // Lignes
    for (int r = 0; r < Board::SIZE; ++r) {
        score += evalTriplet(b, color, {{{0,r},{1,r},{2,r}}});
    }
    // Colonnes
    for (int c = 0; c < Board::SIZE; ++c) {
        score += evalTriplet(b, color, {{{c,0},{c,1},{c,2}}});
    }
    // Diagonales
    score += evalTriplet(b, color, {{{0,0},{1,1},{2,2}}});
    score += evalTriplet(b, color, {{{0,2},{1,1},{2,0}}});

    return score;
}


int minimax(const Position pos, Board &board, const int depth, const bool isMax) {
    int score = 0;
    if (CheckWinService::isWin(board)) {
        score = 1000 * (isMax ? -1 : 1) - depth;
        return score;
    }
    if (depth == 3) {
        return evaluation(board, isMax) - depth;
    }
    const std::vector<Position> positions = posFromPos(pos, board);
    if (positions.empty()) {
        return 0;
    }

    if (isMax) {
        score = std::numeric_limits<int>::min();
        for (auto &position : positions) {
            board.addStoneWhite(position);
            score = std::max(score, minimax(position, board, depth + 1, false));
            //std::cout << "max: " << score << " depth: " << depth << std::endl;
            board.removeWhiteStoneAt(position);
        }
    }
    else {
        score = std::numeric_limits<int>::max();
        for (auto &position : positions) {
            board.addStoneBlack(position);
            score = std::min(score, minimax(position, board, depth + 1, true));
            //std::cout << "min: " << score << " depth: " << depth << std::endl;
            board.removeBlackStoneAt(position);
        }
    }
    return score;
}

Position bestMove(Board& board, const bool color) {
    const std::vector<Position> positions = newPos(board);
    int bestValue = std::numeric_limits<int>::min();
    Position bestPosition = {0, 0};
    for (const auto position : positions) {
        color ? board.addStoneWhite(position) : board.addStoneBlack(position);
        const int tmpValue = minimax(position, board, 0, false);
        std::cout << tmpValue << std::endl;
        color ? board.removeWhiteStoneAt(position) : board.removeBlackStoneAt(position);
        if (tmpValue > bestValue) {
            bestValue = tmpValue;
            bestPosition = position;
        }
    }
    std::cout << bestPosition.x << " " << bestPosition.y << std::endl;
    return bestPosition;
}

AiPlay AIService::AIPlay(Board board) {
    const clock_t start = clock();
    const Position pos = bestMove(board, true);
    const clock_t end = clock();
    const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    return {pos, time};
}

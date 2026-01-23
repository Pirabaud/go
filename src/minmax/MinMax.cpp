#include "MinMax.hpp"

#include <fstream>
#include <iostream>
#include <limits.h>
#include "JsonService.hpp"
#include "CheckWinService.hpp"
#include "HeuristicService.h"

int MinMax::MAX_DEPTH = 1;

MinMax::MinMax(Board &board) : board(board) {
}

MinMax::~MinMax() {
}

Board & MinMax::getBoard() const {
    return board;
}

std::pair<Position, long> MinMax::run(Position playerMove, json& decisionTree, std::vector<Position>& moveHistory) const {
    auto startTime = std::chrono::high_resolution_clock::now();

    int bestValue = INT_MIN;
    Position bestMove{-1, -1};
    // const int rootHeuristic = HeuristicService::getHeuristicValue(TODO, TODO);

    auto possibleMoves = generatePossibleMoves(board);

    for (const auto& move : possibleMoves) {
        int x = move / 64;
        int y = move % 64;
        bestMove = Position{x, y};
    }


       // board.addStoneWhite(move);

    //     json childTree = json::array();
    //     int moveValue = minimax(board, 1, INT_MIN, INT_MAX, true, childTree, move);
    //     //TODO remove stone
    //
    //     if (moveValue > bestValue) {
    //         bestValue = moveValue;
    //         bestMove = move;
    //     }
    // }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    long elapsedMs = duration.count();

    return {bestMove, elapsedMs};
}


int MinMax::minimax(Board& currentBoard, int depth, int alpha, int beta, bool isMaximizing, json& tree, Position pos) {
    // Condition d'arrêt - NE PAS CRÉER DE NŒUD POUR LES FEUILLES
    auto possibleMoves = generatePossibleMoves(currentBoard);
    for (auto move : possibleMoves) {
        int x = move / 64;
        int y = move % 64;
        std::cout << possibleMoves.size() << "x: " << x << " y: " << y << std::endl;
    }
    // if (depth >= MAX_DEPTH) {
    //     //TODO check win and empty
    //     return HeuristicService::getHeuristicValue(currentBoard); // Juste retourner la valeur, pas de nœud créé
    // }
    //
    // int resultValue;
    //
    // if (isMaximizing) {
    //     int maxEval = INT_MIN;
    //     for (const auto& move : possibleMoves) {
    //         //currentBoard.addStoneWhite(move);
    //         json childTree = json::array();
    //         //int eval = minimax(currentBoard, depth + 1, alpha, beta, false, childTree, move);
    //         //TODO remove stone
    //
    //         maxEval = std::max(maxEval, eval);
    //         alpha = std::max(alpha, eval);
    //
    //         if (beta <= alpha) break;
    //     }
    //     resultValue = maxEval;
    // } else {
    //     int minEval = INT_MAX;
    //     for (const auto& move : possibleMoves) {
    //         //currentBoard.addStoneBlack(move);
    //         json childTree = json::array();
    //         //int eval = minimax(currentBoard, depth + 1, alpha, beta, true, childTree, move);
    //         //TODO remove stone
    //
    //         minEval = std::min(minEval, eval);
    //         beta = std::min(beta, eval);
    //
    //         if (beta <= alpha) break;
    //     }
    //     resultValue = minEval;
    // }


    return 0;
}

std::vector<int> MinMax::generatePossibleMoves(Board& currentBoard) {
    std::vector<int> moves;
    if (currentBoard.isEmpty()) {
        moves.push_back(180);
        return moves;
    }

    std::array<uint64_t, 6> occupied;

    for (int i = 0; i < 6; i++) {
        occupied[i] = currentBoard.getBitBoardWhite()[i] | currentBoard.getBitBoardBlack()[i];
    }

    const std::array<uint64_t, 6> right = Board::shift_right_board(occupied, 1);
    const std::array<uint64_t, 6> occupied_right = Board::bitBoardOr(occupied, right);
    const std::array<uint64_t, 6> left = Board::shift_left_board(occupied, 1);
    const std::array<uint64_t, 6> occupied_horizontal = Board::bitBoardOr(occupied_right, left);

    const std::array<uint64_t, 6> top = Board::shift_right_board(occupied_horizontal, 19);
    const std::array<uint64_t, 6> occupied_top = Board::bitBoardOr(occupied_horizontal, top);
    const std::array<uint64_t, 6> down = Board::shift_left_board(occupied_horizontal, 19);
    const std::array<uint64_t, 6> occupied_total = Board::bitBoardOr(occupied_top, down);

    for (int i = 0; i < 6; i++) {
        uint64_t candidates = occupied_total[i] & ~occupied[i];
        while (candidates != 0) {
            int index = i * 64 + std::countr_zero(candidates);
            if (index < 361) {
                moves.push_back(index);
            }
            candidates &= candidates - 1;
        }
    }

    return moves;
}

bool MinMax::isNearExistingStone(Board& board, Position pos, int radius) {
    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
            const int checkX = pos.x + dx;
            int checkY = pos.y + dy;

            if (checkX >= 0 && checkX < Board::SIZE && checkY >= 0 && checkY < Board::SIZE) {
                Position checkPos{checkX, checkY};
                //check autour
            }
        }
    }
    return false;
}
#include "MinMax.hpp"

#include <fstream>
#include <iostream>
#include <limits.h>

#include "JsonService.hpp"
#include "CheckWinService.hpp"
#include "HeuristicService.h"

int MinMax::MAX_DEPTH = 3;

MinMax::MinMax(Board &board) : board(board) {
}

MinMax::~MinMax() = default;

Board & MinMax::getBoard() const {
    return board;
}

std::pair<Position, long> MinMax::run(Position move, json& decisionTree, std::vector<Position>& moveHistory) const {
    const auto startTime = std::chrono::high_resolution_clock::now();

    int bestValue = INT_MIN;
    int bestMoveIndex = -1;
    int currentScore = 0;

    const auto possibleIndeces = generatePossibleMoves(board);
    json children = json::array();
    for (const auto& possibleMoveIndex : possibleIndeces) {

        const int blackScoreBefore = HeuristicService::evaluatePosition(board, possibleMoveIndex, true);
        const int whiteScoreBefore = HeuristicService::evaluatePosition(board, possibleMoveIndex, false);

        // std::cout << "--------------------------" << std::endl;
        // std::cout << board << std::endl;
        board.addStoneWhite(possibleMoveIndex);
        // std::cout << std::endl;
        // std::cout << board << std::endl;


        const int blackScoreAfter = HeuristicService::evaluatePosition(board, possibleMoveIndex, true);
        const int whiteScoreAfter = HeuristicService::evaluatePosition(board, possibleMoveIndex, false);
        const int whiteGain = whiteScoreAfter - whiteScoreBefore;
        const int blackGain = blackScoreAfter - blackScoreBefore;
        const int newScore = currentScore + whiteGain - blackGain;

        // std::cout << "Old board evaluation: " << currentScore << std::endl;
        // std::cout << "Black score before: " << blackScoreBefore << ", after: " << blackScoreAfter << std::endl;
        // std::cout << "White score before: " << whiteScoreBefore << ", after: " << whiteScoreAfter << std::endl;
        // std::cout << "White gain: " << whiteGain << ", Black gain: " << blackGain << std::endl;
        // std::cout << "New score: " << newScore << std::endl;


        json childTree = json::array();


        const int moveValue = minimax(board, 1, INT_MIN, INT_MAX, true, childTree, possibleMoveIndex, newScore);

        // std::cout << moveValue << std::endl;

        int x = possibleMoveIndex / (Board::SIZE + 1);
        int y = possibleMoveIndex % (Board::SIZE + 1);
        JsonService::pushNode(children, moveValue, 1, INT_MIN, INT_MAX, {x,y}, childTree);


        board.removeWhiteStone(possibleMoveIndex);
        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMoveIndex = possibleMoveIndex;
        }
    }


    JsonService::pushNode(decisionTree, 0, 0, INT_MIN, INT_MAX, move, children);
    json historyArray = json::array();
    for (const auto& [x, y] : moveHistory) {
        historyArray.push_back({x, y});
    }
    decisionTree.push_back(historyArray);

    saveDecisionTree(decisionTree);



    const auto endTime = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    const long elapsedMs = duration.count();
    const int bestX = bestMoveIndex / (Board::SIZE + 1);
    const int bestY = bestMoveIndex % (Board::SIZE + 1);
    return {{bestX, bestY}, elapsedMs};
}


int MinMax::minimax(Board& currentBoard, int depth, int alpha, int beta, bool isMaximizing, json& tree, int lastPositionIndex, int currentScore) {
    // Condition d'arrêt - NE PAS CRÉER DE NŒUD POUR LES FEUILLES
    if (depth >= MAX_DEPTH) {
        return currentScore;
    }

    auto possibleMoveIndeces = generatePossibleMoves(currentBoard);


    json children = json::array();


    if (possibleMoveIndeces.empty()) {
        return currentScore;
    }

    int resultValue;

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (const auto& possibleMoveIndex : possibleMoveIndeces) {
            const int blackScoreBefore = HeuristicService::evaluatePosition(currentBoard, possibleMoveIndex, true);
            const int whiteScoreBefore = HeuristicService::evaluatePosition(currentBoard, possibleMoveIndex, false);
            currentBoard.addStoneWhite(possibleMoveIndex);
            const int blackScoreAfter = HeuristicService::evaluatePosition(currentBoard, possibleMoveIndex, true);
            const int whiteScoreAfter = HeuristicService::evaluatePosition(currentBoard, possibleMoveIndex, false);
            const int whiteGain = whiteScoreAfter - whiteScoreBefore;
            const int blackGain = blackScoreAfter - blackScoreBefore;
            const int newScore = currentScore + whiteGain - blackGain;


            json childTree = json::array();


            int eval = minimax(currentBoard, depth + 1, alpha, beta, false, childTree, possibleMoveIndex, newScore);


            int x = possibleMoveIndex / (Board::SIZE + 1);
            int y = possibleMoveIndex % (Board::SIZE + 1);
            JsonService::pushNode(tree, eval, depth + 1, alpha, beta, {x, y}, childTree);


            currentBoard.removeWhiteStone(possibleMoveIndex);

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if (beta <= alpha) break;
        }
        resultValue = maxEval;
    } else {
        int minEval = INT_MAX;
        for (const auto& possibleMoveIndex : possibleMoveIndeces) {
            const int blackScoreBefore = HeuristicService::evaluatePosition(currentBoard, possibleMoveIndex, true);
            const int whiteScoreBefore = HeuristicService::evaluatePosition(currentBoard, possibleMoveIndex, false);
            currentBoard.addStoneBlack(possibleMoveIndex);
            const int blackScoreAfter = HeuristicService::evaluatePosition(currentBoard, possibleMoveIndex, true);
            const int whiteScoreAfter = HeuristicService::evaluatePosition(currentBoard, possibleMoveIndex, false);
            const int whiteGain = whiteScoreAfter - whiteScoreBefore;
            const int blackGain = blackScoreAfter - blackScoreBefore;
            const int newScore = currentScore + whiteGain - blackGain;


            json childTree = json::array();


            int eval = minimax(currentBoard, depth + 1, alpha, beta, true, childTree, possibleMoveIndex, newScore);


            int x = possibleMoveIndex / (Board::SIZE + 1);
            int y = possibleMoveIndex % (Board::SIZE + 1);
            JsonService::pushNode(tree, eval, depth + 1, alpha, beta, {x,y }, childTree);



            currentBoard.removeBlackStone(possibleMoveIndex);

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            if (beta <= alpha) break;
        }
        resultValue = minEval;
    }
    return 0;
}

std::vector<int> MinMax::generatePossibleMoves(Board& currentBoard) {
    std::vector<int> moves{};
    if (currentBoard.isEmpty()) {
        moves.push_back(180);
        return moves;
    }

    std::array<uint64_t, 6> occupied{};

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



void MinMax::saveDecisionTree(const json& tree) {
    std::ofstream file("data.json");
    if (file.is_open()) {
        file << tree.dump(2);  // Pretty print avec indentation
        file.close();
        std::cout << "Arbre de décision sauvegardé dans data.json" << std::endl;
    } else {
        std::cerr << "Erreur: Impossible de sauvegarder l'arbre de décision" << std::endl;
    }
}
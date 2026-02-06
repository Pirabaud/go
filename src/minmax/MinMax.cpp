#include "MinMax.hpp"

#include <fstream>
#include <iostream>
#include <climits>

#include "JsonService.hpp"
#include "CheckWinService.hpp"
#include "HeuristicService.h"


MinMax::MinMax(Board &board) : board(board) {
    transpositionTable = TranspositionTable();
}

MinMax::~MinMax() = default;

Board & MinMax::getBoard() const {
    return board;
}

std::pair<Position, long> MinMax::run(int timeLimitMs, const bool isBlack) {
    this->timeLimit = std::chrono::milliseconds(timeLimitMs - 5);
    this->startTime = std::chrono::high_resolution_clock::now();
    this->timeOut = false;
    this->nodesVisited = 0;

    int globalBestMove = -1;
    int currentBestMove = -1;
    int score = 0;
    int maxDepthReached = 0;

    for (int depth = 1; depth <= 6; ++depth) {
        int val = minmax(board, depth, 0, INT_MIN, INT_MAX, !isBlack, 0, &currentBestMove);

        if (this->timeOut) {
            break;
        }

        maxDepthReached = depth;
        globalBestMove = currentBestMove;
        score = val;
    }
    std::cout << "Reached depth: " << maxDepthReached << ", score: " << score << ", nodes visited: " << nodesVisited << std::endl;
    const auto endTime = std::chrono::high_resolution_clock::now();
    const long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    if (globalBestMove != -1) {
        int x = globalBestMove / (Board::SIZE + 1);
        int y = globalBestMove % (Board::SIZE + 1);
        return {{x, y}, elapsed};
    }

    return {{-1, -1}, elapsed};
}

int MinMax::minmax(Board& currentBoard, int limitDepth, int currentDepth, int alpha, int beta, bool isMaximizing, int currentScore, int* outBestMoveIndex) {
if ((nodesVisited++ & 4095) == 0) {
        checkTime();
    }
    if (this->timeOut) return 0;

    const int remainingDepth = limitDepth - currentDepth;
    const uint64_t zobristKey = currentBoard.currentZobristKey;
    int ttMoveIndex = -1;

    if (auto ttEntry = transpositionTable.retrieve(zobristKey)) {
        if (ttEntry->depth >= remainingDepth) {
            if (ttEntry->flag == EXACT) return ttEntry->score;
            if (ttEntry->flag == LOWERBOUND && ttEntry->score >= beta) return ttEntry->score;
            if (ttEntry->flag == UPPERBOUND && ttEntry->score <= alpha) return ttEntry->score;
        }
        ttMoveIndex = ttEntry->bestMove;
    }

    if (currentDepth >= limitDepth) {
        return currentScore;
    }

    auto possibleMoveIndeces = generatePossibleMoves(currentBoard);
    if (possibleMoveIndeces.empty()) {
        return currentScore;
    }

    if (ttMoveIndex != -1) {
        for (size_t i = 0; i < possibleMoveIndeces.size(); ++i) {
            if (possibleMoveIndeces[i] == ttMoveIndex) {
                std::swap(possibleMoveIndeces[0], possibleMoveIndeces[i]);
                break;
            }
        }
    }

    const int alphaOrig = alpha;
    int bestVal = isMaximizing ? INT_MIN : INT_MAX;
    int localBestMove = -1;
    const bool isWhite = isMaximizing;

    for (const auto& moveIndex : possibleMoveIndeces) {

        const int blackScoreBefore = HeuristicService::evaluatePosition(currentBoard, moveIndex, true);
        const int whiteScoreBefore = HeuristicService::evaluatePosition(currentBoard, moveIndex, false);

        if (isWhite) currentBoard.addStoneWhite(moveIndex);
        else currentBoard.addStoneBlack(moveIndex);

        const int blackScoreAfter = HeuristicService::evaluatePosition(currentBoard, moveIndex, true);
        const int whiteScoreAfter = HeuristicService::evaluatePosition(currentBoard, moveIndex, false);
        const int newScore = currentScore + (whiteScoreAfter - whiteScoreBefore) - (blackScoreAfter - blackScoreBefore);

        int eval = minmax(currentBoard, limitDepth, currentDepth + 1, alpha, beta, !isMaximizing, newScore, nullptr);

        if (isWhite) currentBoard.removeWhiteStone(moveIndex);
        else currentBoard.removeBlackStone(moveIndex);

        if (isMaximizing) {
            if (eval > bestVal) {
                bestVal = eval;
                localBestMove = moveIndex;
            }
            alpha = std::max(alpha, bestVal);
        } else {
            if (eval < bestVal) {
                bestVal = eval;
                localBestMove = moveIndex;
            }
            beta = std::min(beta, bestVal);
        }

        if (beta <= alpha) {
            break;
        }
    }

    if (outBestMoveIndex != nullptr) {
        *outBestMoveIndex = localBestMove;
    }

    if (!this->timeOut) {
        TTFlag flag;
        if (bestVal <= alphaOrig) flag = UPPERBOUND;
        else if (bestVal >= beta) flag = LOWERBOUND;
        else flag = EXACT;

        transpositionTable.store(zobristKey, remainingDepth, bestVal, flag, localBestMove);
    }

    return bestVal;
}

void MinMax::checkTime() {
    const auto now = std::chrono::high_resolution_clock::now();
    if (now - startTime >= timeLimit) {
        timeOut = true;
    }
}

// std::pair<Position, long> MinMax::run(Position move, json& decisionTree, std::vector<Position>& moveHistory) const {
//     const auto startTime = std::chrono::high_resolution_clock::now();
//
//     json rootChildren = json::array();
//
//     int bestMoveIndex = -1;
//
//     int bestScore = minmax(board, 0, INT_MIN, INT_MAX, true, rootChildren, 0, &bestMoveIndex);
//
//     JsonService::pushNode(decisionTree, bestScore, 0, INT_MIN, INT_MAX, move, rootChildren);
//
//     json historyArray = json::array();
//     for (const auto& [x, y] : moveHistory) {
//         historyArray.push_back({x, y});
//     }
//     decisionTree.push_back(historyArray);
//     saveDecisionTree(decisionTree);
//
//     const auto endTime = std::chrono::high_resolution_clock::now();
//     const long elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
//
//     if (bestMoveIndex != -1) {
//         int bestX = bestMoveIndex / (Board::SIZE + 1);
//         int bestY = bestMoveIndex % (Board::SIZE + 1);
//         return {{bestX, bestY}, elapsedMs};
//     }
//
//     return {{-1, -1}, elapsedMs};
// }
//
// int MinMax::minmax(Board& currentBoard, int depth, int alpha, int beta, bool isMaximizing, json& parentTree, int currentScore, int* outBestMoveIndex) {
//     if (depth >= MAX_DEPTH) {
//         return currentScore;
//     }
//
//     auto possibleMoveIndeces = generatePossibleMoves(currentBoard);
//     if (possibleMoveIndeces.empty()) {
//         return currentScore;
//     }
//
//     int bestVal = isMaximizing ? INT_MIN : INT_MAX;
//     int localBestMove = -1;
//
//     bool isWhite = isMaximizing;
//
//     for (const auto& moveIndex : possibleMoveIndeces) {
//
//         const int blackScoreBefore = HeuristicService::evaluatePosition(currentBoard, moveIndex, true);
//         const int whiteScoreBefore = HeuristicService::evaluatePosition(currentBoard, moveIndex, false);
//
//         if (isWhite) {
//             currentBoard.addStoneWhite(moveIndex);
//         } else {
//             currentBoard.addStoneBlack(moveIndex);
//         }
//
//         const int blackScoreAfter = HeuristicService::evaluatePosition(currentBoard, moveIndex, true);
//         const int whiteScoreAfter = HeuristicService::evaluatePosition(currentBoard, moveIndex, false);
//
//         const int whiteGain = whiteScoreAfter - whiteScoreBefore;
//         const int blackGain = blackScoreAfter - blackScoreBefore;
//
//         const int newScore = currentScore + whiteGain - blackGain;
//
//         json childTree = json::array();
//
//         int eval = minmax(currentBoard, depth + 1, alpha, beta, !isMaximizing, childTree, newScore, outBestMoveIndex);
//
//         int x = moveIndex / (Board::SIZE + 1);
//         int y = moveIndex % (Board::SIZE + 1);
//         JsonService::pushNode(parentTree, eval, depth + 1, alpha, beta, {x, y}, childTree);
//
//         if (isWhite) {
//             currentBoard.removeWhiteStone(moveIndex);
//         } else {
//             currentBoard.removeBlackStone(moveIndex);
//         }
//
//         if (isMaximizing) {
//             if (eval > bestVal) {
//                 bestVal = eval;
//                 localBestMove = moveIndex;
//             }
//             alpha = std::max(alpha, bestVal);
//         } else {
//             if (eval < bestVal) {
//                 bestVal = eval;
//                 localBestMove = moveIndex;
//             }
//             beta = std::min(beta, bestVal);
//         }
//
//         if (beta <= alpha) {
//             break;
//         }
//     }
//
//     if (outBestMoveIndex != nullptr) {
//         *outBestMoveIndex = localBestMove;
//     }
//
//     return bestVal;
// }

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

    const std::array<uint64_t, 6> top = Board::shift_right_board(occupied_horizontal, 20);
    const std::array<uint64_t, 6> occupied_top = Board::bitBoardOr(occupied_horizontal, top);
    const std::array<uint64_t, 6> down = Board::shift_left_board(occupied_horizontal, 20);
    const std::array<uint64_t, 6> occupied_total = Board::bitBoardOr(occupied_top, down);

    for (int i = 0; i < 6; i++) {
        uint64_t candidates = occupied_total[i] & ~occupied[i];
        while (candidates != 0) {
            int index = i * 64 + std::countr_zero(candidates);
            if (index <= (Board::SIZE + 1) * 19 - 2) {
                moves.push_back(index);
            }
            candidates &= candidates - 1;
        }
    }
    return moves;
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
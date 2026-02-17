#include "MinMax.hpp"

#include <fstream>
#include <iostream>
#include <climits>

#include "JsonService.hpp"
#include "CheckWinService.hpp"
#include "HeuristicService.h"

// Crée une structure légère ou utilise des tableaux simples


MinMax::MinMax(Board &board) : board(board) {
    transpositionTable = TranspositionTable();
}

MinMax::~MinMax() = default;

Board & MinMax::getBoard() const {
    return board;
}

#include <iostream>
#include <algorithm>
#include <chrono>


std::pair<Position, long> MinMax::run(int timeLimitMs, const bool isBlack) {
    this->timeLimit = std::chrono::milliseconds(timeLimitMs - 15); // Sécurité temps
    this->startTime = std::chrono::high_resolution_clock::now();
    this->timeOut = false;
    this->nodesVisited = 0;

    int globalBestMove = -1;
    int currentBestMove = -1;
    int lastValidScore = 0;
    int maxDepthReached = 0;

    for (int depth = 1; depth <= 12; ++depth) {
        int val = minmax(board, depth, 0, -2000000, 2000000, !isBlack, 0, &currentBestMove);

        if (this->timeOut) break;

        maxDepthReached = depth;
        globalBestMove = currentBestMove;
        lastValidScore = val;

    }

    const auto endTime = std::chrono::high_resolution_clock::now();
    const long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    double seconds = elapsed / 1000.0;
    long nps = (seconds > 0) ? static_cast<long>(nodesVisited / seconds) : nodesVisited;

    std::cout << "Reached depth: " << maxDepthReached
              << " | Nodes: " << nodesVisited
              << " | NPS: " << nps << " nodes/s"
              << " | Time: " << elapsed << "ms" << std::endl;

    if (globalBestMove != -1) {
        int x = globalBestMove / (Board::SIZE + 1);
        int y = globalBestMove % (Board::SIZE + 1);
        return {{x, y}, elapsed};
    }
    return {{-1, -1}, elapsed};
}

int MinMax::minmax(Board& currentBoard, int limitDepth, int currentDepth, int alpha, int beta, bool isMaximizing, int currentScore, int* outBestMoveIndex) {

    // 1. Check Temps & TT (Rien ne change ici)
    if ((nodesVisited++ & 2047) == 0) checkTime(); // Check un peu plus fréquent
    if (this->timeOut) return 0;

    int remainingDepth = limitDepth - currentDepth;
    uint64_t key = currentBoard.currentZobristKey;
    int ttMove = -1;

    if (auto ttEntry = transpositionTable.retrieve(key)) {
        if (ttEntry->depth >= remainingDepth) {
            if (ttEntry->flag == EXACT) return ttEntry->score;
            if (ttEntry->flag == LOWERBOUND && ttEntry->score >= beta) return ttEntry->score;
            if (ttEntry->flag == UPPERBOUND && ttEntry->score <= alpha) return ttEntry->score;
        }
        ttMove = ttEntry->bestMove;
    }

    if (currentDepth >= limitDepth) return currentScore;

    // 2. GÉNÉRATION SANS ALLOCATION (Sur la Stack)
    MoveList list;
    // Modifie ton generatePossibleMoves pour qu'il prenne 'MoveList& list' et fasse 'list.add()'
    generatePossibleMoves(currentBoard, isMaximizing, list);

    if (list.count == 0) return currentScore;

    // 3. SCORING & ORDONNANCEMENT (Zéro vector)
    for (int i = 0; i < list.count; ++i) {
        if (list.moves[i] == ttMove) {
            list.scores[i] = 10000000; // Priorité absolue
        } else {
            // Le Delta est calculé ici pour le tri
            list.scores[i] = HeuristicService::evaluatePosition(currentBoard, list.moves[i], isMaximizing);
        }
    }

    int bestVal = isMaximizing ? -2000000 : 2000000;
    int localBestMove = -1;
    int alphaOrig = alpha;

    for (int i = 0; i < 6; ++i) {

        int bestIdx = i;
        for (int j = i + 1; j < list.count; ++j) {
            if (list.scores[j] > list.scores[bestIdx]) bestIdx = j;
        }
        // Swap manuel (très rapide sur des int)
        std::swap(list.moves[i], list.moves[bestIdx]);
        std::swap(list.scores[i], list.scores[bestIdx]);

        if (i > 8 && remainingDepth < 4 && list.scores[i] < 500) {
             continue; // On saute ce coup, il ne sert à rien
        }
        // ---------------------------------------------------------

        int move = list.moves[i];
        int delta = list.scores[i];

        // Si c'est le coup TT, il faut recalculer le vrai delta car son score était fake (10000000)
        if (move == ttMove) {
            delta = HeuristicService::evaluatePosition(currentBoard, move, isMaximizing);
        }

        // Incrémental Score Update
        int nextScore = isMaximizing ? currentScore + delta : currentScore - delta;

        // Jouer le coup
        if (isMaximizing) currentBoard.addStoneWhite(move);
        else currentBoard.addStoneBlack(move);

        // Récursion
        int eval = minmax(currentBoard, limitDepth, currentDepth + 1, alpha, beta, !isMaximizing, nextScore, nullptr);

        // Annuler le coup
        if (isMaximizing) currentBoard.removeWhiteStone(move);
        else currentBoard.removeBlackStone(move);

        // Alpha-Beta classique
        if (isMaximizing) {
            if (eval > bestVal) { bestVal = eval; localBestMove = move; }
            alpha = std::max(alpha, bestVal);
        } else {
            if (eval < bestVal) { bestVal = eval; localBestMove = move; }
            beta = std::min(beta, bestVal);
        }

        if (beta <= alpha) break;
    }

    // Sauvegarde TT
    if (!this->timeOut) {
        TTFlag flag = (bestVal <= alphaOrig) ? UPPERBOUND : (bestVal >= beta ? LOWERBOUND : EXACT);
        transpositionTable.store(key, remainingDepth, bestVal, flag, localBestMove);
    }

    if (outBestMoveIndex) *outBestMoveIndex = localBestMove;
    return bestVal;
}

void MinMax::checkTime() {
    const auto now = std::chrono::high_resolution_clock::now();
    if (now - startTime >= timeLimit) {
        timeOut = true;
    }
}

void MinMax::generatePossibleMoves(Board& currentBoard, bool isWhite, MoveList &outList) {
    outList.count = 0;
    if (currentBoard.isEmpty()) {
        outList.moves[outList.count++] = 180;
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
                if (outList.count < 256) {
                    outList.moves[outList.count++] = index;
                }
            }
            candidates &= candidates - 1;
        }
    }
}

void MinMax::checkbetterMove(std::array<uint64_t, 6> &allyBitboard, std::array<uint64_t, 6> &ennemyBitboard, std::array<uint64_t, 6> &occupiedBitboard, std::vector<int> &possibleMoves) {
    const std::array<int, 4> directions = {
        HORIZONTAL,
        VERTICAL,
        DIAGONAL_TOP_LEFT,
        DIAGONAL_TOP_RIGHT,
    };

    for (int dir : directions) {
        getFreeThreeAlly(ennemyBitboard, occupiedBitboard, dir, possibleMoves);
    }
    if (!possibleMoves.empty()) {
        return;
    }
    for (int dir : directions) {
        getFreeThreeAlly(allyBitboard, occupiedBitboard, dir, possibleMoves);
    }
}

inline bool isValidAndEmpty(const int idx, const std::array<uint64_t, 6>& occupied) {
    if (idx < 0 || idx >= 64 * 6) return false;

    const int arrayIdx = idx / 64;
    const int bitIdx = idx % 64;

    return !(occupied[arrayIdx] & (1ULL << bitIdx));
}



void MinMax::getFreeThreeAlly(
    const std::array<uint64_t, 6>& currentBoard,
    const std::array<uint64_t, 6>& occupiedBoard,
    const int dir,
    std::vector<int> &possibleMoves
    )
{
    auto shift1 = Board::shift_right_board(currentBoard, dir);
    auto shift2 = Board::shift_right_board(currentBoard, 2 * dir);

    std::array<uint64_t, 6> tripleStones;
    for (int i = 0; i < 6; ++i) {
        tripleStones[i] = currentBoard[i] & shift1[i] & shift2[i];
    }

    for (int i = 0; i < 6; ++i) {
        uint64_t result = tripleStones[i];

        while (result != 0) {
            int bitIndex = std::countr_zero(result);
            int globalIndexXXX = (i * 64) + bitIndex;

            int ext1 = globalIndexXXX - dir;
            int ext2 = globalIndexXXX + (3 * dir);

            bool free1 = isValidAndEmpty(ext1, occupiedBoard);
            bool free2 = isValidAndEmpty(ext2, occupiedBoard);

            if (free1 && free2) {
                possibleMoves.push_back(ext1);
                possibleMoves.push_back(ext2);
            }

            result &= (result - 1);
        }
    }
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
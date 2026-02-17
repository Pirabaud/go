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

    for (int depth = 1; depth <= 10; ++depth) {
        int val = minmax(board, depth, 0, INT_MIN, INT_MAX, !isBlack, 0, &currentBestMove);
        if (this->timeOut) {
            break;
        }

        maxDepthReached = depth;
        globalBestMove = currentBestMove;
        score = val;
        if (score >= 15000) {
            break;
        }
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

    // 1. Transposition Table Lookup
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

    const bool isWhite = isMaximizing;
    auto moves = generatePossibleMoves(currentBoard, isWhite);
    if (moves.empty()) {
        return currentScore;
    }

    // 2. INITIALISATION DES SCORES (Sans trier)
    std::vector<int> moveOrderingScores(moves.size());
    for (size_t i = 0; i < moves.size(); ++i) {
        if (moves[i] == ttMoveIndex) {
            moveOrderingScores[i] = 10000000; // Le coup TT doit être testé en premier absolu
        } else {
            // Heuristique simple pour ordonner les autres coups
            moveOrderingScores[i] = HeuristicService::evaluatePosition(currentBoard, moves[i], isMaximizing);
        }
    }

    const int alphaOrig = alpha;
    int bestVal = isMaximizing ? INT_MIN : INT_MAX;
    int localBestMove = -1;

    // 3. BOUCLE PRINCIPALE AVEC MOVE PICKER
    for (size_t i = 0; i < moves.size(); ++i) {

        // --- OPTIMISATION : Selection Sort on Demand ---
        // On trouve le meilleur coup parmi ceux restants (de i à fin)
        int bestIndex = i;
        int bestScore = moveOrderingScores[i];

        for (size_t j = i + 1; j < moves.size(); ++j) {
            if (moveOrderingScores[j] > bestScore) {
                bestScore = moveOrderingScores[j];
                bestIndex = j;
            }
        }

        // On place le meilleur coup trouvé à l'index courant 'i'
        std::swap(moves[i], moves[bestIndex]);
        std::swap(moveOrderingScores[i], moveOrderingScores[bestIndex]);
        // -----------------------------------------------

        int moveIndex = moves[i]; // C'est le meilleur coup disponible maintenant

        // Calculs des scores avant/après
        const int blackScoreBefore = HeuristicService::evaluatePosition(currentBoard, moveIndex, true);
        const int whiteScoreBefore = HeuristicService::evaluatePosition(currentBoard, moveIndex, false);

        if (isWhite) currentBoard.addStoneWhite(moveIndex);
        else currentBoard.addStoneBlack(moveIndex);

        const int blackScoreAfter = HeuristicService::evaluatePosition(currentBoard, moveIndex, true);
        const int whiteScoreAfter = HeuristicService::evaluatePosition(currentBoard, moveIndex, false);

        // Mise à jour incrémentale du score
        const int newScore = currentScore + (whiteScoreAfter - whiteScoreBefore) - (blackScoreAfter - blackScoreBefore);

        // Vérification de victoire immédiate
        bool isWin = (isWhite && whiteScoreAfter >= 15000) || (!isWhite && blackScoreAfter >= 15000);
        if (isWin) {
            if (isWhite) currentBoard.removeWhiteStone(moveIndex);
            else currentBoard.removeBlackStone(moveIndex);

            if (outBestMoveIndex != nullptr) {
                *outBestMoveIndex = moveIndex;
            }
            return isWhite ? whiteScoreAfter : blackScoreAfter;
        }

        // Appel Récursif
        int eval = minmax(currentBoard, limitDepth, currentDepth + 1, alpha, beta, !isMaximizing, newScore, nullptr);

        // Undo Move
        if (isWhite) currentBoard.removeWhiteStone(moveIndex);
        else currentBoard.removeBlackStone(moveIndex);

        // Alpha-Beta Logic
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

        // CUTOFF : Si on coupe ici, on a économisé le tri des coups suivants (i+1, etc.)
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


std::vector<int> MinMax::generatePossibleMoves(Board& currentBoard, bool isWhite) {
    std::vector<int> moves{};
    if (currentBoard.isEmpty()) {
        moves.push_back(180);
        return moves;
    }

    std::array<uint64_t, 6> occupied{};

    for (int i = 0; i < 6; i++) {
        occupied[i] = currentBoard.getBitBoardWhite()[i] | currentBoard.getBitBoardBlack()[i];
    }



    // checkbetterMove(isWhite ? currentBoard.getBitBoardWhite() : currentBoard.getBitBoardBlack(), isWhite ? currentBoard.getBitBoardBlack() : currentBoard.getBitBoardWhite(), occupied, moves);
    // if (!moves.empty()) {
    //     return moves;
    // }

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
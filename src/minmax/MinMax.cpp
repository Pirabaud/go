#include "MinMax.hpp"

#include <fstream>
#include <iostream>
#include <climits>

#include "AlignmentChecker.hpp"
#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"
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

std::pair<Position, long> MinMax::run(const int timeLimitMs, const bool isBlack) {
    this->timeLimit = std::chrono::milliseconds(timeLimitMs - 15);
    this->startTime = std::chrono::high_resolution_clock::now();
    this->timeOut = false;
    this->nodesVisited = 0;

    int globalBestMove = -1;
    int currentBestMove = -1;
    int score = 0;
    int maxDepthReached = 0;

    for (int depth = 1; depth <= 42; ++depth) {
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

int MinMax::minmax(Board& currentBoard, const int limitDepth, const int currentDepth, int alpha, int beta, const bool isMaximizing, const int currentScore, int* outBestMoveIndex) {
if ((nodesVisited++ & 4095) == 0) {
        checkTime();
    }
    if (this->timeOut) return 0;

    const int remainingDepth = limitDepth - currentDepth;
    const uint64_t zobristKey = currentBoard.currentZobristKey;
    int ttMoveIndex = -1;

    if (const auto ttEntry = transpositionTable.retrieve(zobristKey)) {
        if (ttEntry->depth >= remainingDepth) {
            if (ttEntry->flag == EXACT) {
                if (outBestMoveIndex != nullptr) *outBestMoveIndex = ttEntry->bestMove;
                return ttEntry->score;
            }
            if (ttEntry->flag == LOWERBOUND && ttEntry->score >= beta) return ttEntry->score;
            if (ttEntry->flag == UPPERBOUND && ttEntry->score <= alpha) return ttEntry->score;
        }
        ttMoveIndex = ttEntry->bestMove;
    }

    if (currentDepth >= limitDepth) {
        return currentScore;
    }

    int possibleMoveIndexes[400];
    int numPossibleMoves = generatePossibleMoves(currentBoard, possibleMoveIndexes, isMaximizing);

    if (numPossibleMoves == 0) {
        return currentScore;
    }
    struct MoveData {
        int totalScore;
        int moveIndex;
        int blackScoreBefore;
        int whiteScoreBefore;
    };
    // Au lieu de std::vector, on utilise un tableau fixe de la taille max du plateau
    MoveData rankedMoves[400];
    int moveCount = 0;

    for (int idx = 0; idx < numPossibleMoves; ++idx) {
        int move = possibleMoveIndexes[idx]; // On récupère le vrai coup

        int scoreBlack = HeuristicService::evaluatePosition(currentBoard, move, true);
        int scoreWhite = HeuristicService::evaluatePosition(currentBoard, move, false);

        if (move == ttMoveIndex) {
            rankedMoves[moveCount++] = {1000000, move, scoreBlack, scoreWhite};
        } else {
            int moveScore = scoreBlack + scoreWhite;
            rankedMoves[moveCount++] = {moveScore, move, scoreBlack, scoreWhite};
        }
    }

    // On trie uniquement la portion remplie du tableau (de 0 à moveCount)
    std::sort(rankedMoves, rankedMoves + moveCount, [](const auto& a, const auto& b) {
        return a.totalScore > b.totalScore;
    });

    const int alphaOrig = alpha;
    int bestVal = isMaximizing ? INT_MIN : INT_MAX;
    int localBestMove = -1;
    const bool isWhite = isMaximizing;
    bool firstMove = true;

    // La boucle change légèrement pour parcourir notre tableau fixe
    int MAX_MOVES_TO_TEST;
    if (currentDepth == 0) MAX_MOVES_TO_TEST = 10;      // Racine : On regarde large
    else if (currentDepth <= 5) MAX_MOVES_TO_TEST = 8;  // Profondeur 1-2 : On cible
    else if (currentDepth <= 10) MAX_MOVES_TO_TEST = 4;  // Profondeur 3-5 : Très ciblé
    else MAX_MOVES_TO_TEST = 3;

    int movesTested = 0;

    for (int i = 0; i < moveCount; ++i) {

        // On garde TOUJOURS le coup de la Table de Transposition (c'est le guide !)
        // On garde TOUJOURS les coups avec un gros score (menaces de mort/victoire)
        if (movesTested >= MAX_MOVES_TO_TEST
        && rankedMoves[i].moveIndex != ttMoveIndex
        && rankedMoves[i].totalScore < 1500) // Assure-toi que 10000 correspond bien à une grosse menace chez toi !
        {
        continue; // On jette à la poubelle
        }

        movesTested++;

        const auto& moveData = rankedMoves[i];
        const int moveIndex = moveData.moveIndex;
        // ... suite de ton code (blackScoreBefore, etc.) ...
        int capture[8];
        int countCapture = 0;

        // --- CES DEUX LIGNES ONT DISPARU ! ON UTILISE LES VALEURS SAUVEGARDÉES ---
        const int blackScoreBefore = moveData.blackScoreBefore;
        const int whiteScoreBefore = moveData.whiteScoreBefore;

        // On joue le coup
        if (isWhite) currentBoard.addStoneWhite(moveIndex);
        else currentBoard.addStoneBlack(moveIndex);

        // On calcule juste l'état "After" (Il ne reste plus que 2 appels au lieu de 4 !)
        const int blackScoreAfter = HeuristicService::evaluatePosition(currentBoard, moveIndex, true);
        const int whiteScoreAfter = HeuristicService::evaluatePosition(currentBoard, moveIndex, false);
        const int checkCapture = CaptureService::checkCapture(currentBoard, moveIndex, !isWhite, capture, countCapture);

        const int newScore = currentScore + (whiteScoreAfter - whiteScoreBefore) - (blackScoreAfter - blackScoreBefore);

        int eval;

        // --- 2. PRINCIPAL VARIATION SEARCH (PVS) ---
        if (firstMove) {
            // Le premier coup est testé avec la fenêtre complète (Alpha-Beta normal)
            eval = minmax(currentBoard, limitDepth, currentDepth + 1, alpha, beta, !isMaximizing, newScore, nullptr);
            firstMove = false;
        } else {
            // Pour les autres, on fait une recherche Zéro Fenêtre (bSearch)
            // On teste juste si le coup est strictement meilleur que alpha
            if (isMaximizing) {
                eval = minmax(currentBoard, limitDepth, currentDepth + 1, alpha, alpha + 1, !isMaximizing, newScore, nullptr);
                // Si la recherche rapide échoue (le coup est meilleur que prévu), on refait une vraie recherche
                if (eval > alpha && eval < beta) {
                    eval = minmax(currentBoard, limitDepth, currentDepth + 1, alpha, beta, !isMaximizing, newScore, nullptr);
                }
            } else {
                eval = minmax(currentBoard, limitDepth, currentDepth + 1, beta - 1, beta, !isMaximizing, newScore, nullptr);
                if (eval < beta && eval > alpha) {
                    eval = minmax(currentBoard, limitDepth, currentDepth + 1, alpha, beta, !isMaximizing, newScore, nullptr);
                }
            }
        }

        if (isWhite) currentBoard.removeWhiteStone(moveIndex);
        else currentBoard.removeBlackStone(moveIndex);

        if (checkCapture > 0) {

            if (isWhite) {
                for (int j = 0; j < countCapture; j++) {
                    currentBoard.addStoneBlack(capture[j]);
                }
            }
            else {
                for (int j = 0; j < countCapture; j++) {
                    currentBoard.addStoneWhite(capture[j]);
                }
            }
            currentBoard.removeCaptures(isWhite, countCapture);
        }

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
            break; // Coupure Beta !
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

int MinMax::generatePossibleMoves(Board& currentBoard, int* outMoves, int isMaximize) {
    int moveCount = 0;

    if (currentBoard.isEmpty()) {
        outMoves[moveCount++] = 180; // Centre du plateau
        return moveCount;
    }
    std::array<uint64_t, 6> occupied{};
    for (int i = 0; i < 6; i++) {
        occupied[i] = currentBoard.getBitBoardWhite()[i] | currentBoard.getBitBoardBlack()[i];
    }

    //return if check win

    const std::array<uint64_t, 6> right = Board::shift_right_board(occupied, 1);
    const std::array<uint64_t, 6> occupied_right = Board::bitBoardOr(occupied, right);
    const std::array<uint64_t, 6> left = Board::shift_left_board(occupied, 1);
    const std::array<uint64_t, 6> occupied_horizontal = Board::bitBoardOr(occupied_right, left);

    const std::array<uint64_t, 6> top = Board::shift_right_board(occupied_horizontal, 20);
    const std::array<uint64_t, 6> occupied_top = Board::bitBoardOr(occupied_horizontal, top);
    const std::array<uint64_t, 6> down = Board::shift_left_board(occupied_horizontal, 20);
    const std::array<uint64_t, 6> occupied_total = Board::bitBoardOr(occupied_top, down);

    const std::array<uint64_t, 6> ally = isMaximize ? currentBoard.getBitBoardWhite() : currentBoard.getBitBoardBlack();
    const std::array<uint64_t, 6> enemy = isMaximize ? currentBoard.getBitBoardBlack() : currentBoard.getBitBoardWhite();

    for (int i = 0; i < 6; i++) {
        uint64_t candidates = occupied_total[i] & ~occupied[i];
        while (candidates != 0) {
            const int index = i * 64 + std::countr_zero(candidates);

            // On s'assure de ne PAS dépasser le plateau, ET de ne PAS jouer sur la case fantôme (index % 20 == 19)
            if (index < 380 && (index % 20) != 19) {
                if (CheckLegalMove::isLegalMove(index, currentBoard, isMaximize) != IllegalMoves::Type::NONE) {
                    candidates &= candidates - 1;
                    continue;
                }
                if (AlignmentChecker::checkWinAt(ally, index)) {
                    outMoves[moveCount++] = index;
                    return moveCount;
                }
                outMoves[moveCount++] = index;
            }
            candidates &= candidates - 1;
        }
    }
    return moveCount;
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
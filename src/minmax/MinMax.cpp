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

Board &MinMax::getBoard() const {
    return board;
}

std::pair<Position, long> MinMax::run(const int timeLimitMs, const bool isBlack, int* depthLive, int* outNodesVisited) {
    this->timeLimit = std::chrono::milliseconds(timeLimitMs - 20);
    this->startTime = std::chrono::high_resolution_clock::now();
    this->timeOut = false;
    this->nodesVisited = 0;

    if (board.isEmpty())
    {
        return {{Board::SIZE / 2, Board::SIZE / 2}, 0};
    }

    int globalBestMove = -1;
    int currentBestMove = -1;
    int score = 0;
    int maxDepthReached = 0;

    for (int depth = 1; depth <= 42; ++depth) {
        const int val = minmax(board, depth, 1, INT_MIN, INT_MAX, !isBlack, 0, &currentBestMove);

        if (this->timeOut) {
            break;
        }

        maxDepthReached = depth;
        globalBestMove = currentBestMove;
        score = val;
    }
    const auto endTime = std::chrono::high_resolution_clock::now();
    const long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    if (depthLive) {
        *depthLive = maxDepthReached;
    }
    if (outNodesVisited) {
        *outNodesVisited = nodesVisited;
    }
    if (globalBestMove != -1) {
        const int x = globalBestMove / (Board::SIZE + 1);
        const int y = globalBestMove % (Board::SIZE + 1);
        return {{x, y}, elapsed};
    }

    return {{-1, -1}, elapsed};
}

inline void moveOrdering(
    const Board& currentBoard,
    const std::array<int, 400>& possibleMoveIndexes,
    const int numPossibleMoves,
    const int ttMoveIndex,
    MoveData* outRankedMoves,
    const int maxMovesToTest)
{
    if (numPossibleMoves == 1) {
        outRankedMoves[0] = {1000000, possibleMoveIndexes[0], 0, 0, false, true};
        return;
    }
    for (int idx = 0; idx < numPossibleMoves; ++idx) {
        const int move = possibleMoveIndexes[idx];

        const int scoreBlack = HeuristicService::evaluatePosition(currentBoard, move, true);
        const int scoreWhite = HeuristicService::evaluatePosition(currentBoard, move, false);
        const int moveScore = scoreBlack + scoreWhite;

        const bool isBlk = (scoreBlack >= 20000 || scoreWhite >= 20000);
        const bool isCap = (scoreBlack >= 7000 || scoreWhite >= 7000);
        // -----------------------------

        if (move == ttMoveIndex) {
            outRankedMoves[idx] = {1000000, move, scoreBlack, scoreWhite, isCap, isBlk};
        } else {
            outRankedMoves[idx] = {moveScore, move, scoreBlack, scoreWhite, isCap, isBlk};
        }
    }

    // Le tri partiel ultra-rapide dont on a parlé !
    int movesToSort = std::min(numPossibleMoves, maxMovesToTest);
    std::partial_sort(outRankedMoves, outRankedMoves + movesToSort, outRankedMoves + numPossibleMoves, [](const auto& a, const auto& b) {
        return a.totalScore > b.totalScore;
    });
}

inline void applyMove(Board& board, const int moveIndex, const bool isWhite) {
    if (isWhite) board.addStoneWhite(moveIndex);
    else board.addStoneBlack(moveIndex);
}

inline void undoMove(Board& board, const int moveIndex, const bool isWhite, const int checkCapture, const int* capture, const int countCapture) {
    if (isWhite) board.removeWhiteStone(moveIndex);
    else board.removeBlackStone(moveIndex);

    if (checkCapture > 0) {
        for (int j = 0; j < countCapture; j++) {
            if (isWhite) board.addStoneBlack(capture[j]);
            else board.addStoneWhite(capture[j]);
        }
        board.removeCaptures(isWhite, countCapture);
    }
}

int MinMax::minmax(Board &currentBoard, const int limitDepth, const int currentDepth, int alpha, int beta,
                   const bool isMaximizing, const int currentScore, int *outBestMoveIndex) {
    if ((nodesVisited++ & 4095) == 0) {
        checkTime();
    }
    if (this->timeOut) return 0;


    if (currentDepth >= limitDepth) {
        return currentScore;
    }

    //TRANSPOSITION TABLE
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


    //GENERATE AND ORDERING MOVES
    std::array<int, 400> possibleMoveIndexes = {};
    std::ranges::fill(possibleMoveIndexes, -1);
    const int numPossibleMoves = generatePossibleMoves(currentBoard, possibleMoveIndexes, isMaximizing);
    if (numPossibleMoves == 0) {
        return currentScore;
    }
    int MAX_MOVES_TO_TEST;
    if (currentDepth == 0) MAX_MOVES_TO_TEST = 10;
    else if (currentDepth <= 5) MAX_MOVES_TO_TEST = 8;
    else if (currentDepth <= 10) MAX_MOVES_TO_TEST = 4;
    else MAX_MOVES_TO_TEST = 3;
    struct MoveData {
        int totalScore;
        int moveIndex;
        int blackScoreBefore;
        int whiteScoreBefore;
    };
    MoveData rankedMoves[400];
    moveOrdering(currentBoard, possibleMoveIndexes, numPossibleMoves, ttMoveIndex, rankedMoves, MAX_MOVES_TO_TEST);


    //EVALUATION
    const int alphaOrig = alpha;
    const int betaOrig = beta;
    int bestVal = isMaximizing ? INT_MIN : INT_MAX;
    int localBestMove = -1;
    const bool isWhite = isMaximizing;
    bool firstMove = true;
    int movesTested = 0;

    for (int i = 0; i < numPossibleMoves; ++i) {
        const auto &moveData = rankedMoves[i];
        const int moveIndex = moveData.moveIndex;

        if (CheckLegalMove::isLegalMove(moveIndex, currentBoard, isWhite) != IllegalMoves::Type::NONE) {
            continue;
        }
        if (movesTested >= MAX_MOVES_TO_TEST
            && localBestMove != -1
            && rankedMoves[i].moveIndex != ttMoveIndex
            && rankedMoves[i].totalScore < 1500
            && !rankedMoves[i].isBlocking
            && !rankedMoves[i].isWin
            && !rankedMoves[i].isCapture
            ) {
           continue;
        }

        movesTested++;

        int capture[8];
        int countCapture = 0;

        applyMove(currentBoard, moveData.moveIndex, isWhite);

        const int blackScoreAfter = HeuristicService::evaluatePosition(currentBoard, moveIndex, true);
        const int whiteScoreAfter = HeuristicService::evaluatePosition(currentBoard, moveIndex, false);
        const int checkCapture = CaptureService::checkCapture(currentBoard, moveIndex, !isWhite, capture, countCapture);
        bool isTrueWin = false;
        if ((isWhite && whiteScoreAfter >= 30000) || (!isWhite && blackScoreAfter >= 30000)) {
            if (CheckWinService::isWin(currentBoard) != nullptr) {
                isTrueWin = true;
            }
        }

        int eval;
        if (isTrueWin) {
            if (isWhite) {
                eval = 300000 - currentDepth;
            } else {
                eval = -300000 + currentDepth;
            }
        }
        else {
            int newScore = currentScore + (whiteScoreAfter - moveData.whiteScoreBefore) - (blackScoreAfter - moveData.blackScoreBefore);
            if (checkCapture > 0) {
                const int captureBonus = checkCapture * 8000;
                newScore += isMaximizing ? captureBonus : -captureBonus;
            }
            eval = executePVS(currentBoard, limitDepth, currentDepth, alpha, beta, isMaximizing, newScore, firstMove);
        }
        firstMove = false;
        undoMove(currentBoard, moveData.moveIndex, isWhite, checkCapture, capture, countCapture);
        if (this->timeOut) return 0;

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
    if (bestVal == INT_MIN || bestVal == INT_MAX) {
        return currentScore;
    }

    if (!this->timeOut) {
        TTFlag flag;
        if (bestVal <= alphaOrig) flag = UPPERBOUND;
        else if (bestVal >= betaOrig) flag = LOWERBOUND;
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

int MinMax::generatePossibleMoves(Board &currentBoard, std::array<int, 400>& outMoves, const int isMaximize) {
    int moveCount = 0;

    if (currentBoard.isEmpty()) {
        outMoves[moveCount++] = 189;
        return moveCount;
    }

    const auto blockingBreakableWinMoves = CheckWinService::getWinBlockingIndices(currentBoard, !isMaximize);
    if (blockingBreakableWinMoves[0] != -1) {
        for (int i = 0; i < blockingBreakableWinMoves.size() && blockingBreakableWinMoves[i] != -1; i++) {
            if (CheckLegalMove::isLegalMove(blockingBreakableWinMoves[i], currentBoard, !isMaximize) == IllegalMoves::Type::NONE) {
                outMoves[moveCount++] = blockingBreakableWinMoves[i];
            }
        }
        if (moveCount == 1) {
            return moveCount;
        }
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

    const std::array<uint64_t, 6> top = Board::shift_right_board(occupied_horizontal, Board::SIZE + 1);
    const std::array<uint64_t, 6> occupied_top = Board::bitBoardOr(occupied_horizontal, top);
    const std::array<uint64_t, 6> down = Board::shift_left_board(occupied_horizontal, Board::SIZE + 1);
    const std::array<uint64_t, 6> occupied_total = Board::bitBoardOr(occupied_top, down);

    const std::array<uint64_t, 6> allyBitboard = isMaximize ? currentBoard.getBitBoardWhite() : currentBoard.getBitBoardBlack();

    for (int i = 0; i < 6; i++) {
        uint64_t candidates = occupied_total[i] & ~occupied[i];
        while (candidates != 0) {
            const int index = i * 64 + std::countr_zero(candidates);
            if (index < (Board::SIZE * (Board::SIZE + 1)) && (index % (Board::SIZE + 1)) != Board::SIZE) {
                if (CheckLegalMove::isLegalMove(index, currentBoard, !isMaximize) != IllegalMoves::Type::NONE) {
                    candidates &= candidates - 1;
                    continue;
                }
                if (AlignmentChecker::checkWinAt(allyBitboard, index)) {
                     outMoves[0] = index;
                     return 1;
                 }
                outMoves[moveCount++] = index;
            }
            candidates &= candidates - 1;
        }
    }
    return moveCount;
}

inline int MinMax::executePVS(Board &currentBoard, const int limitDepth, const int currentDepth,
                              const int alpha, const int beta, const bool isMaximizing,
                              const int newScore, const bool firstMove) {
    if (firstMove) {
        return minmax(currentBoard, limitDepth, currentDepth + 1, alpha, beta, !isMaximizing, newScore, nullptr);
    }

    int eval;
    if (isMaximizing) {
        eval = minmax(currentBoard, limitDepth, currentDepth + 1, alpha, alpha + 1, !isMaximizing, newScore, nullptr);
        if (eval > alpha && eval < beta) {
            eval = minmax(currentBoard, limitDepth, currentDepth + 1, alpha, beta, !isMaximizing, newScore, nullptr);
        }
    } else {
        eval = minmax(currentBoard, limitDepth, currentDepth + 1, beta - 1, beta, !isMaximizing, newScore, nullptr);
        if (eval < beta && eval > alpha) {
            eval = minmax(currentBoard, limitDepth, currentDepth + 1, alpha, beta, !isMaximizing, newScore, nullptr);
        }
    }
    return eval;
}

void MinMax::saveDecisionTree(const json &tree) {
    std::ofstream file("data.json");
    if (file.is_open()) {
        file << tree.dump(2); // Pretty print avec indentation
        file.close();
        std::cout << "Arbre de décision sauvegardé dans data.json" << std::endl;
    } else {
        std::cerr << "Erreur: Impossible de sauvegarder l'arbre de décision" << std::endl;
    }
}

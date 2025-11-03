#include "MinMax.hpp"
#include "MoveGenerator.hpp"
#include "FastEvaluator.hpp"
#include <iostream>
#include <limits>
#include <algorithm>

int MinMax::MAX_DEPTH = 10;

MinMax::MinMax(Board& board, bool playingWhite)
    : board(board)
    , isWhite(playingWhite)
    , ttable(128)
    , moveOrdering()
    , zobrist(ZobristHash::getInstance())
    , nodesSearched(0)
    , timeLimit(5000)
    , timeExpired(false)
{
}

MinMax::~MinMax() {
}

std::pair<Position, long> MinMax::run(int timeLimitMs) {
    auto start = std::chrono::high_resolution_clock::now();
    startTime = start;
    timeLimit = timeLimitMs;
    timeExpired = false;
    nodesSearched = 0;

    Position bestMove = iterativeDeepening(MAX_DEPTH, timeLimitMs);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    long elapsedMs = duration.count();

    std::cout << "IA joue en (" << bestMove.x << ", " << bestMove.y << ") "
              << "| Nodes: " << nodesSearched
              << " | Time: " << elapsedMs << "ms" << std::endl;

    return {bestMove, elapsedMs};
}

Position MinMax::iterativeDeepening(int maxDepth, int timeLimitMs) {
    Position bestMove{-1, -1};
    int bestScore = std::numeric_limits<int>::min();

    moveOrdering.clearHistory();

    int stoneCount = 0;
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 0; y < Board::SIZE; y++) {
            if (board.isWhiteStoneAt({x, y}) || board.isBlackStoneAt({x, y})) {
                stoneCount++;
            }
        }
    }

    for (int depth = 1; depth <= maxDepth; depth++) {
        if (isTimeUp()) {
            std::cout << "Time limit reached at depth " << depth - 1 << std::endl;
            break;
        }

        uint64_t hash = zobrist.getHash(board);
        int searchRadius = 2;  // Augmenté pour capturer les positions défensives éloignées
        std::vector<Position> moves = MoveGenerator::generateMoves(board, searchRadius);

        if (moves.empty()) {
            moves.push_back({Board::SIZE / 2, Board::SIZE / 2});
            return moves[0];
        }

        Position hashMove{-1, -1};
        int dummyScore;
        ttable.probe(hash, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), dummyScore, hashMove);

        moveOrdering.orderMoves(moves, 0, hashMove, board, isWhite);

        Position depthBestMove = moves[0];
        int depthBestScore = std::numeric_limits<int>::min();

        int alpha = std::numeric_limits<int>::min();
        int beta = std::numeric_limits<int>::max();

        if (depth > 1 && bestMove.x != -1) {
            alpha = bestScore - 100000;
            beta = bestScore + 100000;
        }

        bool needFullSearch = false;

        for (const auto& move : moves) {
            if (isTimeUp()) break;

            auto undo = board.makeMove(move, isWhite);
            uint64_t newHash = zobrist.updateHash(hash, move, isWhite);

            int score;

            if (needFullSearch || depth <= 1) {
                score = -pvs(board, depth - 1, -beta, -alpha, !isWhite, move, newHash);
            } else {
                score = -pvs(board, depth - 1, -beta, -alpha, !isWhite, move, newHash);
                if (score <= alpha || score >= beta) {
                    needFullSearch = true;
                    alpha = std::numeric_limits<int>::min();
                    beta = std::numeric_limits<int>::max();
                    score = -pvs(board, depth - 1, -beta, -alpha, !isWhite, move, newHash);
                }
            }

            // CRITICAL: Add massive bonus for immediate captures at root level
            // This ensures we ALWAYS take free captures, even if deep search finds false wins
            int captureCount = undo.capturedStones.size() / 2;
            if (captureCount > 0) {
                score += 2000000;  // Large enough to beat any score (including WIN_SCORE)
            }

            board.undoMove(undo);

            if (score > depthBestScore) {
                depthBestScore = score;
                depthBestMove = move;
            }

            alpha = std::max(alpha, score);
        }

        if (!timeExpired) {
            bestMove = depthBestMove;
            bestScore = depthBestScore;

            ttable.store(hash, bestScore, depth, NodeType::EXACT, bestMove);

            std::cout << "Depth " << depth << " complete | Best move: (" << bestMove.x << ", " << bestMove.y << ") | Score: " << bestScore << std::endl;
        }
    }

    return bestMove;
}

int MinMax::pvs(Board& board, int depth, int alpha, int beta, bool currentPlayerIsWhite, Position lastMove, uint64_t hash) {
    nodesSearched++;
    bool isMaximizing = currentPlayerIsWhite;  // White maximise, Black minimise

    if (isTimeUp()) {
        return FastEvaluator::evaluate(board);
    }

    int alphaOrig = alpha;
    Position ttMove{-1, -1};
    int ttScore;

    if (ttable.probe(hash, depth, alpha, beta, ttScore, ttMove)) {
        return ttScore;
    }

    if (depth <= 0 || FastEvaluator::isWinningPosition(board, !currentPlayerIsWhite)) {
        int score = FastEvaluator::evaluateMove(board, lastMove, !currentPlayerIsWhite);
        if (abs(score) < 100000) {
            score += FastEvaluator::evaluate(board);
        }

        // Add depth penalty for distant gains - prefer immediate captures over distant wins
        // depth = MAX_DEPTH at root, depth = 0 deep in tree
        // So (MAX_DEPTH - depth) = moves away from root
        int movesAway = MAX_DEPTH - depth;
        if (abs(score) < 900000 && movesAway > 0) {  // Don't penalize actual win positions
            int penalty = movesAway * 150;
            score = score > 0 ? score - penalty : score + penalty;
        }

        return score;
    }

    std::vector<Position> moves = MoveGenerator::generateMoves(board, 2);  // Radius 2 pour couverture tactique

    if (moves.empty()) {
        return FastEvaluator::evaluate(board);
    }

    moveOrdering.orderMoves(moves, MAX_DEPTH - depth, ttMove, board, isMaximizing);

    Position bestMove = moves[0];
    bool firstMove = true;

    if (isMaximizing) {
        int maxEval = std::numeric_limits<int>::min();

        for (const auto& move : moves) {
            if (isTimeUp()) break;

            auto undo = board.makeMove(move, currentPlayerIsWhite);
            uint64_t newHash = zobrist.updateHash(hash, move, currentPlayerIsWhite);
            int eval;

            if (firstMove) {
                eval = -pvs(board, depth - 1, -beta, -alpha, !currentPlayerIsWhite, move, newHash);
                firstMove = false;
            } else {
                eval = -pvs(board, depth - 1, -alpha - 1, -alpha, !currentPlayerIsWhite, move, newHash);
                if (eval > alpha && eval < beta) {
                    eval = -pvs(board, depth - 1, -beta, -eval, !currentPlayerIsWhite, move, newHash);
                }
            }

            board.undoMove(undo);

            if (eval > maxEval) {
                maxEval = eval;
                bestMove = move;
            }

            alpha = std::max(alpha, eval);

            if (beta <= alpha) {
                moveOrdering.addKillerMove(MAX_DEPTH - depth, move);
                moveOrdering.updateHistory(move, depth);
                break;
            }
        }

        NodeType type = (maxEval <= alphaOrig) ? NodeType::UPPER_BOUND :
                        (maxEval >= beta) ? NodeType::LOWER_BOUND : NodeType::EXACT;
        ttable.store(hash, maxEval, depth, type, bestMove);

        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();

        for (const auto& move : moves) {
            if (isTimeUp()) break;

            auto undo = board.makeMove(move, currentPlayerIsWhite);
            uint64_t newHash = zobrist.updateHash(hash, move, currentPlayerIsWhite);
            int eval;

            if (firstMove) {
                eval = -pvs(board, depth - 1, -beta, -alpha, !currentPlayerIsWhite, move, newHash);
                firstMove = false;
            } else {
                eval = -pvs(board, depth - 1, -alpha - 1, -alpha, !currentPlayerIsWhite, move, newHash);
                if (eval > alpha && eval < beta) {
                    eval = -pvs(board, depth - 1, -beta, -eval, !currentPlayerIsWhite, move, newHash);
                }
            }

            board.undoMove(undo);

            if (eval < minEval) {
                minEval = eval;
                bestMove = move;
            }

            beta = std::min(beta, eval);

            if (beta <= alpha) {
                moveOrdering.addKillerMove(MAX_DEPTH - depth, move);
                moveOrdering.updateHistory(move, depth);
                break;
            }
        }

        NodeType type = (minEval >= beta) ? NodeType::LOWER_BOUND :
                        (minEval <= alpha) ? NodeType::UPPER_BOUND : NodeType::EXACT;
        ttable.store(hash, minEval, depth, type, bestMove);

        return minEval;
    }
}

bool MinMax::isTimeUp() {
    if (timeExpired) return true;

    if (nodesSearched % 1000 == 0) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        if (elapsed.count() >= timeLimit * 0.93) {
            timeExpired = true;
            return true;
        }
    }

    return false;
}

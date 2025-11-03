#ifndef MINMAX_HPP
#define MINMAX_HPP

#include "Board.h"
#include "Position.hpp"
#include "TranspositionTable.hpp"
#include "MoveOrdering.hpp"
#include "ZobristHash.hpp"
#include <chrono>

class MinMax {
public:
    static int MAX_DEPTH;

    explicit MinMax(Board& board, bool playingWhite = true);
    ~MinMax();

    Board& getBoard() const { return board; }
    std::pair<Position, long> run(int timeLimitMs = 5000);

private:
    Board& board;
    bool isWhite;  // Couleur que l'IA joue
    TranspositionTable ttable;
    MoveOrdering moveOrdering;
    ZobristHash& zobrist;

    int nodesSearched;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    int timeLimit;
    bool timeExpired;

    Position iterativeDeepening(int maxDepth, int timeLimitMs);

    int pvs(Board& board, int depth, int alpha, int beta, bool currentPlayerIsWhite, Position lastMove, uint64_t hash);

    bool isTimeUp();
};

#endif

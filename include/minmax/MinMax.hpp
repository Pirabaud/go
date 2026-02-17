#ifndef MINMAX_HPP
#define MINMAX_HPP

#include "Board.h"
#include "JsonService.hpp"
#include "TranspositionTable.hpp"

struct MoveList {
    int moves[256]; // Tableau fixe (sur la stack = gratuit en temps)
    int scores[256];
    int count = 0;

    void add(int move, int score) {
        moves[count] = move;
        scores[count] = score;
        count++;
    }
};

class MinMax {
public:
    explicit MinMax(Board& board);
    ~MinMax();

    Board& getBoard() const;

    std::pair<Position, long> run(int timeLimitMs, bool isBlack);

    int minmax(Board &currentBoard, int limitDepth, int currentDepth, int alpha, int beta,
                      bool isMaximizing,
                      int currentScore, int *outBestMoveIndex) ;

    void checkTime();

    // std::pair<Position, long> run(Position move, json& decisionTree, std::vector<Position>& moveHistory) const;
    //
    // static int minmax(Board &currentBoard, int depth, int alpha, int beta, bool isMaximizing, json &parentTree,
    //            int currentScore,
    //            int *outBestMoveIndex) ;

    static void generatePossibleMoves(Board &currentBoard, bool isWhite, MoveList &outList);
    static void saveDecisionTree(const json& tree);
private:
    Board& board;

    TranspositionTable transpositionTable;

    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::milliseconds timeLimit;
    static void getFreeThreeAlly(
    const std::array<uint64_t, 6>& currentBoard,
    const std::array<uint64_t, 6>& occupiedBoard,
    const int dir,
    std::vector<int> &possibleMoves
    );
    static void checkbetterMove(std::array<uint64_t, 6> &allyBitboard, std::array<uint64_t, 6> &ennemyBitboard, std::array<uint64_t, 6> &
                                occupiedBitboard, std::vector<int> &possibleMoves);

    bool timeOut = false;
    long nodesVisited = 0;
};


#endif //MINMAX_HPP
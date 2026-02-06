#ifndef MINMAX_HPP
#define MINMAX_HPP

#include "Board.h"
#include "JsonService.hpp"
#include "TranspositionTable.hpp"


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

    static std::vector<int> generatePossibleMoves(Board& currentBoard);
    static void saveDecisionTree(const json& tree);
private:
    Board& board;

    TranspositionTable transpositionTable;

    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::milliseconds timeLimit;

    bool timeOut = false;
    long nodesVisited = 0;
};


#endif //MINMAX_HPP
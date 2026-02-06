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

    // Gestion du temps pour l'Iterative Deepening
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::milliseconds timeLimit;

    // Atomic n'est pas strictement nécessaire en mono-thread, mais c'est une bonne habitude
    bool timeOut = false;
    long nodesVisited = 0;
};


#endif //MINMAX_HPP
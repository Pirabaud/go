#ifndef MINMAX_HPP
#define MINMAX_HPP

#include "Board.h"
#include "JsonService.hpp"


class MinMax {
public:
    static int MAX_DEPTH;

    explicit MinMax(Board& board);
    ~MinMax();

    Board& getBoard() const;
    std::pair<Position, long> run(Position playerMove, json& decisionTree, std::vector<Position>& moveHistory) const;

    static std::vector<Position> generatePossibleMoves(Board& currentBoard);

    static bool isNearExistingStone(Board& board, Position pos, int radius);

    static int minimax(Board& currentBoard, int depth, int alpha, int beta, bool isMaximizing, json& tree);

    static void saveDecisionTree(const json& tree);
private:
    Board& board;
};


#endif //MINMAX_HPP
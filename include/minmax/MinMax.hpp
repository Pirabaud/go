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
    Position run(Position playerMove, json& decisionTree) const;
    std::vector<Position> generatePossibleMoves(Board& currentBoard) const;
    int minimax(Board& currentBoard, int depth, int alpha, int beta, bool isMaximizing, json& tree) const;
    void saveDecisionTree(const json& tree) const;
private:
    Board& board;
};


#endif //MINMAX_HPP
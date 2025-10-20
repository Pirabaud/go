//
// Created by Pierre Rabaud on 17/10/2025.
//

#ifndef GOMOKU_EVALUATION_HPP
#define GOMOKU_EVALUATION_HPP
#include "Board.h"


class EvaluationService {
public:
    static int evaluate(const Board& board, Position pos, bool color);

private:
    static int checkAlignmentEvaluation(Board board, Position pos, bool color);
    static int checlAllAlignmentOpp(Board board, Position pos, bool color);

};


#endif //GOMOKU_EVALUATION_HPP
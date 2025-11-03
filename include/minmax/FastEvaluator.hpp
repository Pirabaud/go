#ifndef FAST_EVALUATOR_HPP
#define FAST_EVALUATOR_HPP

#include "Board.h"
#include "Position.hpp"

class FastEvaluator {
public:
    static constexpr int WIN_SCORE = 1000000;
    static constexpr int CAPTURE_WIN_SCORE = 500000;
    static constexpr int IMMEDIATE_CAPTURE = 600000;  // Increased to prioritize over deep wins
    static constexpr int FOUR_OPEN = 50000;
    static constexpr int FOUR_SEMI = 20000;
    static constexpr int CAPTURE_PAIR = 25000;
    static constexpr int THREE_OPEN = 10000;
    static constexpr int THREE_SEMI = 3000;
    static constexpr int TWO_OPEN = 800;
    static constexpr int CAPTURE_THREAT = 5000;

    static int evaluate(const Board& board);
    static int evaluateMove(Board& board, Position move, bool isWhite);
    static bool isWinningPosition(const Board& board, bool isWhite);
    static bool canCapture(const Board& board, Position move, bool isWhite);
    static int countCapturesForMove(const Board& board, Position move, bool isWhite);

private:
    static int evaluateCaptures(const Board& board);
    static int evaluatePatterns(const Board& board, Position move, bool isWhite);
};

#endif

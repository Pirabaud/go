#ifndef MINMAX_HPP
#define MINMAX_HPP
#include "Board.h"
#include "GameState.hpp"


class MinMax {
public:
    static int MAX_DEPTH;

    explicit MinMax(Board& board);
    ~MinMax();

    Board& getBoard();
    Position run() const;
private:
    Board& board;
    GameState *root = nullptr;
};


#endif //MINMAX_HPP
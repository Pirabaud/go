#ifndef MINMAX_HPP
#define MINMAX_HPP
#include "Board.h"


class MinMax {
public:
    static int MAX_DEPTH;

    explicit MinMax(Board& board);
    ~MinMax();

    Board& getBoard() const;
    Position run() const;
private:
    Board& board;
};


#endif //MINMAX_HPP
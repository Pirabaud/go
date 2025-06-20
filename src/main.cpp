#include <iostream>
#include <bits/ostream.tcc>

#include "Board.h"

int main() {
    Board board;

    board.addStoneWhite(0, 2);
    std::cout << board;
    std::cout << "Hello World!\n" << std::endl;
}

#include <iostream>
#include <bits/ostream.tcc>

#include "Board.h"
#include "Service/CheckMoveService.h"

int main() {
    Board board;

    board.addStoneWhite(0, 1);
    board.addStoneBlack(0, 3);
    std::cout << board;
    // std::cout << CheckMoveService::isLegalMove(-1, 0, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    // std::cout << CheckMoveService::isLegalMove(0, 20, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    // std::cout << CheckMoveService::isLegalMove(0, 0, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    // std::cout << CheckMoveService::isLegalMove(0, 2, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    std::cout << CheckMoveService::isLegalMove(0, 0, board.getGridWhite(), board.getGridBlack(), false) << '\n';

    std::cout << "Hello World!\n" << std::endl;
}

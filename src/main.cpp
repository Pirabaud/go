#include <iostream>
#include <bits/ostream.tcc>

#include "Board.h"
#include "services/CheckMoveService.h"

int main() {
    Board board;

    board.addStoneWhite(1, 2);
    board.addStoneBlack(3, 0);
    board.addStoneBlack(0, 3);
    // std::cout << CheckMoveService::isLegalMove(-1, 0, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    // std::cout << CheckMoveService::isLegalMove(0, 20, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    // std::cout << CheckMoveService::isLegalMove(0, 0, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    // std::cout << CheckMoveService::isLegalMove(0, 2, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    std::cout << CheckMoveService::isLegalMove(2, 1, board.getGridBlack(), board.getGridWhite(), false) << std::endl;
}

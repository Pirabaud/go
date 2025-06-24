#include <iostream>
#include <bits/ostream.tcc>

#include "Board.h"
#include "Service/CheckMoveService.h"

int main() {
    Board board;

    board.addStoneWhite(0, 6);
    board.addStoneBlack(0, 5);
    board.addStoneBlack(0, 4);
    // std::cout << CheckMoveService::isLegalMove(-1, 0, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    // std::cout << CheckMoveService::isLegalMove(0, 20, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    // std::cout << CheckMoveService::isLegalMove(0, 0, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    // std::cout << CheckMoveService::isLegalMove(0, 2, board.getGridWhite(), board.getGridBlack(), false) << '\n';
    std::cout << CheckMoveService::isLegalMove(0, 0, board.getGridWhite(), board.getGridBlack(), false) << std::endl;
}

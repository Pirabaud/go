#include <iostream>

#include "Board.h"
#include "services/CheckMoveService.h"
#include "DisplayService.hpp"
#include "SFML/Window/Window.hpp"

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
int main()
{
    std::cout << "Hello World!\n" << std::endl;
    DisplayService displayService;
    displayService.start();


    std::cout << "Goodbye World!\n" << std::endl;
    return 0;
}

#include "Board.hpp"
#include "DisplayService.hpp"
#include "HeuristicService.hpp"

int main() {
    HeuristicService::init();
    Board::initZobrist();

    DisplayService displayService;
    displayService.start();
}

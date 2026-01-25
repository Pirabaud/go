#include <iostream>

#include "Board.h"
#include "CaptureService.hpp"
#include "services/CheckLegalMove.hpp"
#include "DisplayService.hpp"
#include "HeuristicService.h"
#include "SFML/Window/Window.hpp"
int main() {
    // Initialize Heuristic Service
    HeuristicService::init();
    // Initialize Zobrist Hashing
    Board::initZobrist();

    DisplayService displayService;

    displayService.start();
}

#include <iostream>

#include "Board.h"
#include "CaptureService.hpp"
#include "services/CheckLegalMove.hpp"
#include "DisplayService.hpp"
#include "HeuristicService.h"
#include "SFML/Window/Window.hpp"
int main() {
    DisplayService displayService;

    // Initialize patterns and scores
    HeuristicService::getHeuristicValue(0);




    displayService.start();
}

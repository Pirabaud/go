#include <iostream>

#include "Board.h"
#include "CaptureService.hpp"
#include "services/CheckLegalMove.hpp"
#include "DisplayService.hpp"
#include "HeuristicService.h"
#include "SFML/Window/Window.hpp"
int main() {
    DisplayService displayService;

    // Trigger heuristic values generation
    HeuristicService::getHeuristicValues();


    displayService.start();
}

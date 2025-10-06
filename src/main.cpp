#include <iostream>

#include "Board.h"
#include "CaptureService.hpp"
#include "services/CheckLegalMove.hpp"
#include "DisplayService.hpp"
#include "SFML/Window/Window.hpp"
int main() {
    DisplayService displayService;
    displayService.start();
}

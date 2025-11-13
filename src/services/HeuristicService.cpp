#include "HeuristicService.h"
#include <cmath>
#include <iostream>

#include "AlignmentChecker.hpp"
#include "CheckWinService.hpp"
#include "FreeThreeService.h"
#include "CaptureService.hpp"

#define WIN_WEIGHT 1000000
#define CAPTURE_WEIGHT 8000
#define CAPTURE_THREAT_WEIGHT 2000
#define IMMEDIATE_CAPTURE_WEIGHT 15000  // Capture possible au prochain coup
#define FOUR_OPEN_WEIGHT 900000
#define THREE_OPEN_WEIGHT 50000
#define TWO_OPEN_WEIGHT 100


int getHeuristicFromPos(Board &board, Position pos, bool isBlack) {

    int result = 0;

    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto& [dx, dy] : directions) {

        Alignment alignment = AlignmentChecker::detectAlignment({pos.x, pos.y}, grid, gridOpposite, {dx, dy});

        if (alignment.nbAlignment == 3 && alignment.block == BlockState::FREE) {

            countFreeThree++;
        }

        if (countFreeThree >= 2) {
            return true;
        }
    }

}
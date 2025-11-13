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


int HeuristicService::getHeuristicValue(Board &board) {

    int result = 0;

    for (int i = 0; i < Board::SIZE; i++) {
        for (int j = 0; j < Board::SIZE; j++) {
            if (board.isBlackStoneAt({i,j})) {
                result -= getHeuristicFromPos(board, {i,j}, true);
            }
            else if (board.isWhiteStoneAt( {i,j})) {
                result += getHeuristicFromPos(board, {i,j}, false);
            }
        }
    }
    return result;
}

int HeuristicService::getHeuristicFromPos(Board &board, const Position pos, const bool isBlack) {

    const Board::StoneMask grid = isBlack ? board.getGridBlack() : board.getGridWhite();
    const Board::StoneMask oppositeGrid = isBlack ? board.getGridWhite() : board.getGridBlack();

    return countAlignmentHeuristic(board, pos, grid, oppositeGrid) - countAlignmentHeuristic(board, pos, oppositeGrid, grid);


    }

int HeuristicService::countAlignmentHeuristic(Board &board, const Position pos, const Board::StoneMask &grid, const Board::StoneMask &oppositeGrid) {

    int result = 0;

    const std::array directions = {
        Position(0, 1),
        Position(1, 0),
        Position(1, -1),
        Position(1, 1),
    };

    for (auto& dir : directions) {
        const Alignment alignment = AlignmentChecker::detectAlignment(pos, grid, oppositeGrid, dir);

        if (alignment.block == BLOCKED || alignment.block == NOTALIGN) {
            continue;
        }

        int alignmentScore = 0;

        switch (alignment.nbAlignment) {
            case 5 : alignmentScore += WIN_WEIGHT; break;
            case 4 : alignmentScore += FOUR_OPEN_WEIGHT; break;
            case 3 : alignmentScore += THREE_OPEN_WEIGHT; break;
            default : alignmentScore += TWO_OPEN_WEIGHT; break;
        }

        if (alignment.block == BlockState::SEMIBLOCKED) {
            alignmentScore /= 2 ;
        }
        result+=alignmentScore;
    }
    return result;
}
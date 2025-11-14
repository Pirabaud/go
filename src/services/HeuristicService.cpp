#include "HeuristicService.h"
#include <cmath>
#include <iostream>

#include "AlignmentChecker.hpp"
#include "CheckWinService.hpp"
#include "FreeThreeService.h"
#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"

#define WIN_WEIGHT 1000000000000
#define CAPTURE_WEIGHT 8000
#define CAPTURE_THREAT_WEIGHT 2000
#define IMMEDIATE_CAPTURE_WEIGHT 15000  // Capture possible au prochain coup
#define FOUR_OPEN_WEIGHT 900000
#define THREE_OPEN_WEIGHT 50000
#define TWO_OPEN_WEIGHT 100

#define BLOCK_FACTOR_NO_BLOCK 1
#define BLOCK_FACTOR_ONE_DISTANCE 10
#define BLOCK_FACTOR_TWO_DISTANCE 5
#define BLOCK_FACTOR_THREE_BLOCK 3
#define BLOCK_FACTOR_FOUR_BLOCK 2


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

    // std::cout << pos.x << " " << pos.y << std::endl;
    // std::cout << "grid : " << std::endl;
    // board.printGrid(grid);
    // std::cout << "oppositeGrid" << std::endl;
    // board.printGrid(oppositeGrid);


    return countAlignmentHeuristic(board, pos, grid, oppositeGrid) ;
    }

int HeuristicService::countAlignmentHeuristicOpponent(Board &board, const Position pos, const Board::StoneMask &grid, const Board::StoneMask &oppositeGrid) {
    const int result = 0;

    const std::array directions = {
        Position(0, 1),
        Position(1, 0),
        Position(1, -1),
        Position(1, 1),
    };

    for (auto& dir : directions) {
        int cx = pos.x + dir.x;
        int cy = pos.y + dir.y;
        for (int i = 0; i < 3; i++) {
            // if (!CheckLegalMove::notInBoard({cx, cy}) || Board::isStoneAt(oppositeGrid, {cx, cy})) {
            //     continue;
            // }
            // Alignment alignment = AlignmentChecker::detectAlignment(pos, oppositeGrid, grid, dir);
            // if (alignment.block == BLOCKED || alignment.block == NOTALIGN) {
            //     continue;
            // }
            //
            // int alignmentScore = 0;
            //
            // if (alignment.nbRealAlignment > 1) {
            //     switch (alignment.nbRealAlignment) {
            //         case 5 : alignmentScore -= WIN_WEIGHT / i; break;
            //         case 4 : alignmentScore -= FOUR_OPEN_WEIGHT / i; break;
            //         case 3 : alignmentScore -= THREE_OPEN_WEIGHT / i; break;
            //         default : alignmentScore -= TWO_OPEN_WEIGHT / i; break;
            //     }
            // }
            //
            // if (alignment.block == BlockState::SEMIBLOCKED) {
            //     alignmentScore /= 2 ;
            // }
            // result+=alignmentScore;
        }
    }
    return result;
}

int HeuristicService::getBlockFactor(int blockDistance) {
    switch (blockDistance) {
        case 0: return BLOCK_FACTOR_NO_BLOCK;
        case 1: return BLOCK_FACTOR_ONE_DISTANCE;
        case 2: return BLOCK_FACTOR_TWO_DISTANCE;
        case 3: return BLOCK_FACTOR_THREE_BLOCK;
        case 4: return BLOCK_FACTOR_FOUR_BLOCK;
        default: return BLOCK_FACTOR_NO_BLOCK;
    }
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

        if (alignment.blockDistanceRight >= 1 && alignment.blockDistanceLeft >= 1 && alignment.holeCount == 0) {
            continue;
        }

        int alignmentScore = 0;
        if (alignment.nbRealAlignment > 1) {
            switch (alignment.nbRealAlignment) {
                case 5 : alignmentScore += WIN_WEIGHT; break;
                case 4 : alignmentScore += FOUR_OPEN_WEIGHT; break;
                case 3 : alignmentScore += THREE_OPEN_WEIGHT; break;
                default : alignmentScore += TWO_OPEN_WEIGHT; break;
            }
        }

        int blockFactor = getBlockFactor(alignment.blockDistanceLeft) + getBlockFactor(alignment.blockDistanceRight);

        result += (alignmentScore / (alignment.holeCount + 1) / blockFactor);
    }


    // if (result == 50000) {
    //     std::cout << pos.x << " " << pos.y << " : " << std::endl;
    //     board.printBoard();
    // }
    return result;
}
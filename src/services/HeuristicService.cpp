#include "HeuristicService.h"
#include <cmath>
#include <iostream>
#include <bits/fs_fwd.h>

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

/* Masque de 3 bits (valeur 7) */
#define MASK_3  0x07  // Binaire: 00111

/* Masque de 4 bits (valeur 15) */
#define MASK_4  0x0F  // Binaire: 011110

/* Masque de 5 bits (valeur 31) */
#define MASK_5  0x1F  // Binaire: 11111


int evaluatingLine(const uint32_t line, const uint32_t lineOpponent, int j) {
    int result = 0;
    //if (lineOpponent != 0)
         //std::cout << "lineOpponent :" << lineOpponent << " line: " << line  << " line :" << j << std::endl;
    for (int i = 0; i < 15; i++) {
        uint32_t windows = (line >> i) & MASK_5;
        uint32_t windowsOpponent = lineOpponent >> (i + 1)  & MASK_5;
        uint32_t mask = line >> (i + 1) & MASK_5 | windowsOpponent;
        if (windows == 28 && j == 1) {
            std::cout << windows << " " << i << " " << line << std::endl;
            std::cout << windowsOpponent << " " << i << " " << lineOpponent << std::endl;
            std::cout << mask << std::endl;
        }
        if (windows == 30) {
            result += 50000;
        }
        else if ( windows == 28) {
            i+=4;
            if (mask == 14)
            // std::cout << j << std::endl;
            // std::cout << line << " " <<  lineOpponent << " " << windowsOpponent << " " << windows << std::endl;
            // std::cout << "coucou" << std::endl;
            result += 30000;

        }

        else if (windows == 3) {
            result += 10000;
        }
    }
    //std::cout << result << std::endl;
    return result;
}

int HeuristicService::getHeuristicValue(Board &board) {

    int score = 0;

    for (int i = 0; i < Board::SIZE; i++) {
        //std::cout << "i :" << i << std::endl;
        score += evaluatingLine(board.getLineGridWhite().at(i), board.getLineGridBlack().at(i), i);
        //std::cout << "i :" << score << std::endl;
        score += evaluatingLine(board.getColGridWhite().at(i), board.getColGridBlack().at(i), i);
        //std::cout << "line black :" << std::endl;
        if (i == 1) {
            board.printLineAndCol(board.getLineGridWhite());
        }
        score -= evaluatingLine(board.getLineGridBlack().at(i), board.getLineGridWhite().at(i), i);
        //std::cout << "i :" << i << " line black :" << score << std::endl;
        score -= evaluatingLine(board.getColGridBlack().at(i), board.getColGridWhite().at(i), i);

    }
    for (int i = 0; i < Board::SIZE * 2; i++) {
        // score += evaluatingLine(board.getDiagRightGridWhite().at(i), board.getDiagRightGridBlack().at(i), i);
        // score += evaluatingLine(board.getDiagLeftGridWhite().at(i), board.getDiagLeftGridBlack().at(i), i);
        // score -= evaluatingLine(board.getDiagRightGridBlack().at(i), board.getDiagRightGridWhite().at(i), i);
        // score -= evaluatingLine(board.getDiagLeftGridBlack().at(i), board.getDiagLeftGridWhite().at(i), i);
    }
    return score;
}

int HeuristicService::getHeuristicFromPos(Board &board, const Position pos, const bool isBlack) {
    // int score = 0;
    // if (isBlack) {
    //     score += evaluatingLine(board.getLineGridBlack().at(pos.x));
    //     score += evaluatingLine(board.getColGridBlack().at(pos.y));
    //     score += evaluatingLine(board.getDiagRightGridBlack().at(pos.x + pos.y));
    //     score += evaluatingLine(board.getDiagLeftGridBlack().at(pos.x - pos.y + Board::SIZE - 1));
    //     return score;
    // }
    // score += evaluatingLine(board.getLineGridWhite().at(pos.x));
    // score += evaluatingLine(board.getColGridWhite().at(pos.y));
    // score += evaluatingLine(board.getDiagRightGridWhite().at(pos.x + pos.y));
    // score += evaluatingLine(board.getDiagLeftGridWhite().at(pos.x - pos.y + Board::SIZE - 1));

    //return score;
    
    
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
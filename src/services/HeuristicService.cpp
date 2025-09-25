//
// Created by pirabaud on 9/25/25.
//

#include "HeuristicService.h"

#include "AlignmentChecker.hpp"
#include "CheckWinService.hpp"
#include "FreeThreeService.h"

#define WIN_WEIGHT 1000000
#define FREE_THREE_WEIGHT 1000
#define DOUBLE_WEIGHT 100

/**
 * Compute heuristic value based on pos and stone masks
 * @return value of the heuristic compute (the more the number is negative, the better is for black.
 * the more the number is positive, the better is for white.)
 */
int HeuristicService::getHeuristicValue(Board& board)  {
    // TODO ajout de l'importance des captures
    // TODO modifier l'ancienne valeur heuristique en fonction des alignements qu'on a bloqué autour de notre position
    int heuristicValue = 0;
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            const bool factor = board.isBlackStoneAt({x, y}) ? -1 : 1;
            // Win 100%
            if (AlignmentChecker::detectAlignment({x, y}, 5, board.getGridWhite(), board.getGridBlack()) == Alignment::FREE ||
                AlignmentChecker::detectAlignment({x, y}, 4, board.getGridWhite(), board.getGridBlack()) == Alignment::FREE) {
                return WIN_WEIGHT ;
            }
            if (AlignmentChecker::detectAlignment({x, y}, 5, board.getGridBlack(), board.getGridWhite()) == Alignment::FREE ||
                AlignmentChecker::detectAlignment({x, y}, 4, board.getGridBlack(), board.getGridWhite()) == Alignment::FREE) {
                return -WIN_WEIGHT;
                }
            if (AlignmentChecker::detectAlignment({x, y}, 3, board.getGridWhite(), board.getGridBlack()) == Alignment::FREE) {
                heuristicValue += FREE_THREE_WEIGHT ;
            }
            if (AlignmentChecker::detectAlignment({x, y}, 3, board.getGridBlack(), board.getGridWhite()) == Alignment::FREE) {
                heuristicValue += -FREE_THREE_WEIGHT;
            }
            if (AlignmentChecker::detectAlignment({x, y}, 2, board.getGridWhite(), board.getGridBlack()) == Alignment::FREE) {
                heuristicValue += DOUBLE_WEIGHT ;
            }
            if (AlignmentChecker::detectAlignment({x, y}, 2, board.getGridBlack(), board.getGridWhite()) == Alignment::FREE) {
                heuristicValue += -DOUBLE_WEIGHT;
            }

        }
    }
    return heuristicValue;
}

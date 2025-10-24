//
// Created by pirabaud on 9/25/25.
//

#include "HeuristicService.h"

#include "AlignmentChecker.hpp"
#include "CheckWinService.hpp"
#include "FreeThreeService.h"

// #define WIN_WEIGHT 1000000
// #define FREE_THREE_WEIGHT 1000
// #define DOUBLE_WEIGHT 100

#define WIN_WEIGHT 100
#define TWO_IN_ROW_WEIGHT 10
#define ONE_IN_ROW_WEIGHT 1

int HeuristicService::evaluateLines(Board& board) {
    int score = 0;

    // Lignes horizontales
    for (int y = 0; y < Board::SIZE; ++y) {
        score += evaluateLine(board, 0, y, 1, 0); // →
    }

    // Lignes verticales
    for (int x = 0; x < Board::SIZE; ++x) {
        score += evaluateLine(board, x, 0, 0, 1); // ↓
    }

    // Diagonales
    score += evaluateLine(board, 0, 0, 1, 1);  // ↘
    score += evaluateLine(board, 2, 0, -1, 1); // ↙

    return score;
}

int HeuristicService::evaluateLine(Board& board, int startX, int startY, int dx, int dy) {
    int whiteCount = 0, blackCount = 0, emptyCount = 0;

    for (int i = 0; i < Board::SIZE; ++i) {
        int x = startX + i * dx;
        int y = startY + i * dy;
        Position pos{x, y};

        if (board.isWhiteStoneAt(pos)) {
            whiteCount++;
        } else if (board.isBlackStoneAt(pos)) {
            blackCount++;
        } else {
            emptyCount++;
        }
    }

    // Évaluer la ligne
    if (whiteCount == 3) return WIN_WEIGHT;
    if (blackCount == 3) return -WIN_WEIGHT;

    if (whiteCount == 2 && emptyCount == 1) return TWO_IN_ROW_WEIGHT;
    if (blackCount == 2 && emptyCount == 1) return -TWO_IN_ROW_WEIGHT;

    if (whiteCount == 1 && emptyCount == 2) return ONE_IN_ROW_WEIGHT;
    if (blackCount == 1 && emptyCount == 2) return -ONE_IN_ROW_WEIGHT;

    return 0;
}

/**
 * Compute heuristic value based on pos and stone masks
 * @return value of the heuristic compute (the more the number is negative, the better is for black.
 * the more the number is positive, the better is for white.)
 */
int HeuristicService::getHeuristicValue(Board& board)  {

    // Vérifier la victoire d'abord
    const sf::Color* winner = CheckWinService::isWin(board);
    if (winner) {
        return (*winner == sf::Color::White) ? WIN_WEIGHT : -WIN_WEIGHT;
    }

    int heuristicValue = 0;

    // Évaluer toutes les lignes, colonnes et diagonales
    heuristicValue += evaluateLines(board);

    return heuristicValue;

    /*
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
            Alignment whiteDoubleAlignment = AlignmentChecker::detectAlignment({x, y}, 2, board.getGridWhite(), board.getGridBlack());
            if (whiteDoubleAlignment == Alignment::FREE) {
                heuristicValue += DOUBLE_WEIGHT ;
            } else if (whiteDoubleAlignment == Alignment::SEMIBLOCKED) {
                heuristicValue += DOUBLE_WEIGHT / 2 ;
            }
            Alignment blackDoubleAlignment = AlignmentChecker::detectAlignment({x, y}, 2, board.getGridBlack(), board.getGridWhite());
            if (blackDoubleAlignment == Alignment::FREE) {
                heuristicValue += DOUBLE_WEIGHT ;
            } else if (blackDoubleAlignment == Alignment::SEMIBLOCKED) {
                heuristicValue += DOUBLE_WEIGHT / 2 ;
            }

        }
    }
    return heuristicValue;
    */
}

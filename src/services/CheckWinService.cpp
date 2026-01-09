#include "../../include/services/CheckWinService.hpp"

#include <bitset>
#include <iostream>
#include <ostream>
#include <vector>

#include "AlignmentChecker.hpp"
#include "Direction.hpp"
#include "LineBlockState.hpp"


const sf::Color* CheckWinService::isWin(Board& board)
{


    if (AlignmentChecker::checkWinAlignment(board.getBitBoardWhite(), board.getBitBoardBlack(), Direction::HORIZONTAL) ||
        AlignmentChecker::checkWinAlignment(board.getBitBoardWhite(), board.getBitBoardBlack(), Direction::VERTICAL) ||
        AlignmentChecker::checkWinAlignment(board.getBitBoardWhite(), board.getBitBoardBlack(), Direction::DIAGONAL_TOP_LEFT) ||
        AlignmentChecker::checkWinAlignment(board.getBitBoardWhite(), board.getBitBoardBlack(), Direction::DIAGONAL_TOP_RIGHT)) {
        return &sf::Color::White;
    }
    if (AlignmentChecker::checkWinAlignment(board.getBitBoardBlack(),board.getBitBoardWhite(), Direction::HORIZONTAL) ||
        AlignmentChecker::checkWinAlignment(board.getBitBoardBlack(),board.getBitBoardWhite(), Direction::VERTICAL) ||
        AlignmentChecker::checkWinAlignment(board.getBitBoardBlack(),board.getBitBoardWhite(), Direction::DIAGONAL_TOP_LEFT) ||
        AlignmentChecker::checkWinAlignment(board.getBitBoardBlack(),board.getBitBoardWhite(), Direction::DIAGONAL_TOP_RIGHT)) {
        return &sf::Color::Black;
        }

    if (board.getBlackCaptured() >= 10) {
        return &sf::Color::White;
    }
    if (board.getWhiteCaptured() >= 10) {
        return &sf::Color::Black;
    }
    return nullptr;
}

bool CheckWinService::isBreakableWinAlignment(Board &allyBoard, Board &enemyBoard, int startIndex, Direction dir) {
    return false;
}


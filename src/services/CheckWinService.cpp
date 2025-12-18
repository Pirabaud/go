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

    if (AlignmentChecker::check5Alignment(board.getBitBoardWhite(), Direction::HORIZONTAL) ||
        AlignmentChecker::check5Alignment(board.getBitBoardWhite(), Direction::VERTICAL) ||
        AlignmentChecker::check5Alignment(board.getBitBoardWhite(), Direction::DIAGONAL_TOP_LEFT) ||
        AlignmentChecker::check5Alignment(board.getBitBoardWhite(), Direction::DIAGONAL_TOP_RIGHT)) {
        return &sf::Color::White;
    }

    if (AlignmentChecker::check5Alignment(board.getBitBoardBlack(),Direction::HORIZONTAL) ||
        AlignmentChecker::check5Alignment(board.getBitBoardBlack(),Direction::VERTICAL) ||
        AlignmentChecker::check5Alignment(board.getBitBoardBlack(),Direction::DIAGONAL_TOP_LEFT) ||
        AlignmentChecker::check5Alignment(board.getBitBoardBlack(),Direction::DIAGONAL_TOP_RIGHT)) {
        return &sf::Color::Black;
        }


    //TODO Add condition win if a player has captured 10 opponents stones
    if (board.getBlackCaptured() >= 10) {
        return &sf::Color::White;
    }
    if (board.getWhiteCaptured() >= 10) {
        return &sf::Color::Black;
    }
    return nullptr;
}


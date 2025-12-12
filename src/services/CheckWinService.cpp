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

    if (AlignmentChecker::check5Alignment(board.getBitBoardWhite(), Direction::HORIZONTAL_RIGHT) ||
        AlignmentChecker::check5Alignment(board.getBitBoardWhite(), Direction::VERTICAL_TOP) ||
        AlignmentChecker::check5Alignment(board.getBitBoardWhite(), Direction::DIAGONAL_TOP_LEFT) ||
        AlignmentChecker::check5Alignment(board.getBitBoardWhite(), Direction::DIAGONAL_TOP_RIGHT)) {
        return &sf::Color::White;
    }

    if (AlignmentChecker::check5Alignment(board.getBitBoardBlack(),Direction::HORIZONTAL_RIGHT) ||
        AlignmentChecker::check5Alignment(board.getBitBoardBlack(),Direction::VERTICAL_TOP) ||
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

bool CheckWinService::isColorWin(const Board::StoneMask& grid)
{
    //TODO check autour de la pierre posée
    //TODO voir pour optimiser cette fonction avec un pattern different par direction


    for (int y = 0; y < Board::SIZE; ++y)
    {
        for (int x = 0; x < Board::SIZE; ++x)
        {
            if (isWinAtPos(grid, {x, y})) {
                return true;
            }
        }
    }
    return false;
}

bool CheckWinService::isWinAtPos(const Board::StoneMask& grid, Position pos) {

    const std::array directions = {
        Position{1, 0},
        Position{0, 1},
        Position{1, 1},
        Position{-1, 1}
    };

    for (auto [dx, dy] : directions)
    {
        int count = 0;
        int cx = pos.x, cy = pos.y;
        while (cx >= 0 && cx < Board::SIZE && cy >= 0 && cy < Board::SIZE &&
            ((grid[cy] >> cx) & 1))
        {
            count++;
            if (count == WIN_LENGTH_CONDITION) return true;
            cx += dx;
            cy += dy;
        }
    }
    return false;
}


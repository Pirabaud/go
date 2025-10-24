#include "../../include/services/CheckWinService.hpp"

#include <vector>

const sf::Color* CheckWinService::isWin(Board& board)
{
    //TODO Add condition win if a player has captured 10 opponents stones

    if (isColorWin(board.getGridBlack()))
    {
        return &sf::Color::Black;
    }
    if (isColorWin(board.getGridWhite()))
    {
        return &sf::Color::White;
    }
    return nullptr;
}

bool CheckWinService::isColorWin(const Board::StoneMask& grid)
{
    //TODO check autour de la pierre posée
    //TODO voir pour optimiser cette fonction avec un pattern different par direction

    const std::vector<std::pair<int, int>> directions = {
        {1, 0}, // →
        {0, 1}, // ↓
        {1, 1}, // ↘
        {-1, 1} // ↙
    };

    for (int y = 0; y < Board::SIZE; ++y)
    {
        for (int x = 0; x < Board::SIZE; ++x)
        {
            for (auto [dx, dy] : directions)
            {
                int count = 0;
                int cx = x, cy = y;
                while (cx >= 0 && cx < Board::SIZE && cy >= 0 && cy < Board::SIZE &&
                    ((grid[cy] >> cx) & 1))
                {
                    count++;
                    if (count == 3) return true;
                    cx += dx;
                    cy += dy;
                }
            }
        }
    }
    return false;
}


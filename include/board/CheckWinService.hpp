//
// Created by afaby on 6/20/25.
//

#ifndef CHECK_WIN_SERVICE_HPP
#define CHECK_WIN_SERVICE_HPP
#include "Board.h"
#include "SFML/Graphics/Color.hpp"


class CheckWinService
{
public:
    static const sf::Color* isWin(const Board& board);

private:
    CheckWinService() = default;

    static bool isColorWin(const Board::StoneMask& grid);
};


#endif //CHECK_WIN_SERVICE_HPP

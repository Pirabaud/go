//
// Created by afaby on 6/20/25.
//

#ifndef CHECK_WIN_SERVICE_HPP
#define CHECK_WIN_SERVICE_HPP
#include "../board/Board.h"
#include "SFML/Graphics/Color.hpp"

#define WIN_LENGTH_CONDITION 5

class CheckWinService
{
public:
    static const sf::Color* isWin(Board &board);

private:
    CheckWinService() = default;

    static bool isColorWin(const Board::StoneMask& grid);
};


#endif //CHECK_WIN_SERVICE_HPP

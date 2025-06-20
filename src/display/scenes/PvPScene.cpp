#include "PvPScene.hpp"

#include <iostream>

#include "CheckWinService.hpp"

void PvPScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    const bool doesStoneHaveBeenPlaced = handleStonePlacement(event, window);
    if (doesStoneHaveBeenPlaced)
    {
        const sf::Color* winningColor = CheckWinService::isWin(board);
        if (winningColor)
        {
            std::cout << "Player " << (*winningColor == sf::Color::White ? "White" : "Black") << " wins!" << std::endl;
        }
    }
}

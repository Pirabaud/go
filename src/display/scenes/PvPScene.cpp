#include "PvPScene.hpp"

#include <iostream>

#include "CheckWinService.hpp"
#include "SFML/Graphics/Text.hpp"
#include "utils/getSharedFont.hpp"

void PvPScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    if (winningColor) return;
    const bool doesStoneHaveBeenPlaced = handleStonePlacement(event, window);
    if (doesStoneHaveBeenPlaced)
    {
        winningColor = CheckWinService::isWin(board);
        if (winningColor)
        {
            std::cout << "Player " << (*winningColor == sf::Color::White ? "White" : "Black") << " wins!" << std::endl;
        }
    }
}

void PvPScene::drawTexts(sf::RenderWindow& window)
{
    if (winningColor)
    {
        sf::Text winText(getSharedFont(),
                         "Player " + std::string(*winningColor == sf::Color::White ? "White" : "Black") + " wins!");

        winText.setCharacterSize(18);
        winText.setFillColor(sf::Color::White);
        winText.setPosition({BOARD_SIZE_WITH_PADDING, PADDING});

        window.draw(winText);
    }
}


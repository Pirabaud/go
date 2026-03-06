#include "PvPScene.hpp"

#include <iostream>

#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"
#include "../../../include/services/CheckWinService.hpp"
#include "display/utils/getSharedFont.hpp"
#include "SFML/Graphics/Text.hpp"

void PvPScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (winningColor) return;
    const bool doesStoneHaveBeenPlaced = handleStonePlacement(event, window);
    if (doesStoneHaveBeenPlaced) {
        winningColor = CheckWinService::isWin(board);
        if (winningColor) {
            std::cout << "Player " << (*winningColor == sf::Color::White ? "White" : "Black") << " wins!" << std::endl;
        }
        draw(window);
        if (!winningColor) {
            std::cout << "Run suggestion as " << (colorToPlay == sf::Color::Black ? "Black" : "White") << std::endl;
            this->suggestedMove = handleAITurn();
        }
    }
}

void PvPScene::drawTexts(sf::RenderWindow& window) {

    if (illegalMove != IllegalMoves::Type::NONE) {
        sf::Text illegalMoveText(getSharedFont(),
                                 "This is move is illegal because " + std::string(IllegalMoves::toString(illegalMove)) +
                                 ".");
        illegalMoveText.setCharacterSize(18);
        illegalMoveText.setFillColor(sf::Color::Red);
        illegalMoveText.setPosition({BOARD_SIZE_WITH_PADDING, PADDING + 20});

        window.draw(illegalMoveText);
    }


    if (winningColor) {
        sf::Text winText(getSharedFont(),
                         "Player " + std::string(*winningColor == sf::Color::White ? "White" : "Black") + " wins!");

        winText.setCharacterSize(30);
        winText.setFillColor(sf::Color::White);
        winText.setPosition({static_cast<float>(DisplayService::WINDOW_WIDTH / 2 - 100), static_cast<float>(DisplayService::WINDOW_HEIGHT / 2 - 50)});

        sf::Text newGameText(getSharedFont(),
                         "Press ESC to go to main menu.");

        newGameText.setCharacterSize(18);
        newGameText.setFillColor(sf::Color::White);
        newGameText.setPosition({static_cast<float>(DisplayService::WINDOW_WIDTH / 2 - 110), static_cast<float>(DisplayService::WINDOW_HEIGHT / 2)});

        sf::RectangleShape fadeBackground(sf::Vector2f(DisplayService::WINDOW_WIDTH, DisplayService::WINDOW_HEIGHT));
        fadeBackground.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
        fadeBackground.setPosition({0, 0});

        window.draw(fadeBackground);
        window.draw(winText);
        window.draw(newGameText);
    }
}

bool PvPScene::handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    int captures[8];
    int count = 0;
    if (!event || !event->is<sf::Event::MouseButtonPressed>()) {
        return false;
    }

    if (const auto& mousePressedEvent = event->getIf<sf::Event::MouseButtonPressed>(); mousePressedEvent &&
        mousePressedEvent->button == sf::Mouse::Button::Left) {
        const auto& mousePos = mousePressedEvent->position;
        const auto [row, col] = getCellFromMousePosition(mousePos);
        if (row == -1 || col == -1) {
            illegalMove = IllegalMoves::Type::NOT_IN_BOARD;
            return false;
        }
        illegalMove = CheckLegalMove::isLegalMove(Board::getGlobalIndex({row, col}), board, colorToPlay == sf::Color::Black);

        if (illegalMove != IllegalMoves::Type::NONE) {
            return false;
        }
        CaptureService::checkCapture(
            board,
            Board::getGlobalIndex({row, col}),
            colorToPlay == sf::Color::Black, captures, count
        );
        playMove(Position{row, col});
        return true;
    }
    return false;
}

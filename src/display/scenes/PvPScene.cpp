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
    }
}

void PvPScene::drawTexts(sf::RenderWindow& window) {

    if (winningColor) {
        sf::Text winText(getSharedFont(),
                         "Player " + std::string(*winningColor == sf::Color::White ? "White" : "Black") + " wins!");

        winText.setCharacterSize(18);
        winText.setFillColor(sf::Color::White);
        winText.setPosition({BOARD_SIZE_WITH_PADDING, PADDING});

        window.draw(winText);
    }
    if (illegalMove != IllegalMoves::Type::NONE) {
        sf::Text illegalMoveText(getSharedFont(),
                                 "This is move is illegal because " + std::string(IllegalMoves::toString(illegalMove)) +
                                 ".");
        illegalMoveText.setCharacterSize(18);
        illegalMoveText.setFillColor(sf::Color::Red);
        illegalMoveText.setPosition({BOARD_SIZE_WITH_PADDING, PADDING + 20});

        window.draw(illegalMoveText);
    }
}

bool PvPScene::handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
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
        illegalMove = CheckLegalMove::isLegalMove(Position{row, col}, board, colorToPlay == sf::Color::Black);

        if (illegalMove != IllegalMoves::Type::NONE) {
            return false;
        }
        playMove(Position{row, col});
        CaptureService::checkCapture(
            colorToPlay == sf::Color::Black ? board.getBitBoardBlack() : board.getBitBoardWhite(),
        colorToPlay == sf::Color::Black ? board.getBitBoardWhite() : board.getBitBoardBlack(),
            Position{row, col}
        );
        return true;
    }
    return false;
}


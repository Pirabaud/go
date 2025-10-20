//
// Created by Pierre Rabaud on 06/10/2025.
//

#include "PvEScene.hpp"
#include "CheckWinService.hpp"
#include "SFML/Graphics/Text.hpp"

#include <iostream>

#include "AiPlay.hpp"
#include "AIService.hpp"
#include "CheckLegalMove.hpp"
#include "utils/getSharedFont.hpp"

void PvEScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (winningColor || AITurn) return;
    playerPlay = handleStonePlacement(event, window);
    if (playerPlay) {
        winningColor = CheckWinService::isWin(board);
        if (winningColor) {
            std::cout << "Player " << (*winningColor == sf::Color::White ? "White" : "Black") << " wins!" << std::endl;
        }
    }
}

void PvEScene::Ai(sf::RenderWindow& window) {
    if (playerPlay) {
        winningColor = CheckWinService::isWin(board);
        if (winningColor) {
            std::cout << "AI " << (*winningColor == sf::Color::White ? "White" : "Black") << " wins!" << std::endl;
        }
        else {
            AITurn = true;
            AIPlay();
            AITurn = false;
        }
        playerPlay = false;
    }
}

void PvEScene::drawTexts(sf::RenderWindow& window) {

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
    sf::Text illegalMoveText(getSharedFont(),
   "ai time to play: " + std::string(std::to_string(aiPlay.Time)) +
       " seconds.");
        illegalMoveText.setCharacterSize(18);
        illegalMoveText.setFillColor(sf::Color::Green);
        illegalMoveText.setPosition({BOARD_SIZE_WITH_PADDING, PADDING + 80});
        window.draw(illegalMoveText);
}

bool PvEScene::handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
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
        board.resolveCaptures();
        return true;
        }
    return false;
}

bool PvEScene::AIPlay() {
    aiPlay = AIService::AIPlay(board);
    playMove(aiPlay.pos);
    board.resolveCaptures();
    return true;
}


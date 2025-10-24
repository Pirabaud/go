//
// Created by Pierre Rabaud on 06/10/2025.
//

#include "PvEScene.hpp"
#include "CheckWinService.hpp"
#include "SFML/Graphics/Text.hpp"

#include <iostream>

#include "CheckLegalMove.hpp"
#include "DisplayService.hpp"
#include "HeuristicService.h"
#include "utils/getSharedFont.hpp"
#include "JsonService.hpp"

void PvEScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (winningColor) return;

    if (handleStonePlacement(event, window)) {
        winningColor = CheckWinService::isWin(board);
        if (winningColor) {
            std::cout << "Player " << (*winningColor == sf::Color::White ? "White" : "Black") << " wins!" << std::endl;
        }
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
    sf::Text AITimeMs(getSharedFont(),
                         "Time taken : " + (colorToPlay == sf::Color::Black ? std::to_string(lastAITimeMs) + " ms" : "playing..."));
    AITimeMs.setCharacterSize(18);
    AITimeMs.setFillColor(lastAITimeMs >= 500 ? sf::Color::Red : sf::Color(0,150,0));
    AITimeMs.setPosition({BOARD_SIZE_WITH_PADDING, DisplayService::WINDOW_HEIGHT - PADDING - 20});

    window.draw(AITimeMs);
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
        Position playerMove = Position(row, col);

        playMove(playerMove);

        std::cout << "Heuristic : " << HeuristicService::getHeuristicValue(board) << std::endl;
        board.resolveCaptures();
        std::cout << "Black captured: " << board.getBlackCaptured() << std::endl;
        std::cout << "White captured: " << board.getWhiteCaptured() << std::endl;
        draw(window);
        if (CheckWinService::isWin(board)) {
            return true;
        }

        json decisionTree = json::array();
        moveHistory.push_back(playerMove);
        handleAITurn(playerMove, decisionTree, moveHistory);
        board.resolveCaptures();
        board.resolveCaptures();
        std::cout << "Black captured: " << board.getBlackCaptured() << std::endl;
        std::cout << "White captured: " << board.getWhiteCaptured() << std::endl;
        return true;
        }
    return false;
}

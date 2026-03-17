#include "PvEScene.hpp"
#include "CheckWinService.hpp"
#include "SFML/Graphics/Text.hpp"

#include <iostream>

#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"
#include "DisplayService.hpp"
#include "HeuristicService.h"
#include "utils/getSharedFont.hpp"
#include "JsonService.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

void PvEScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    if (winningColor) return;

    if (handleStonePlacement(event, window))
    {
        winningColor = CheckWinService::isWin(board);
        if (winningColor && *winningColor == sf::Color::White && loseSound)
        {
            loseSound->play();
        } else if (winningColor && winSound)
        {
            winSound->play();
        }
    }
}

void PvEScene::drawTexts(sf::RenderWindow& window)
{
    sf::Text AITimeMs(getSharedFont(),
                      "Time taken : " + (colorToPlay == sf::Color::Black
                                             ? std::to_string(lastAITimeMs) + " ms"
                                             : "playing..."));
    AITimeMs.setCharacterSize(18);
    AITimeMs.setFillColor(lastAITimeMs >= 500 ? sf::Color::Red : sf::Color(0, 150, 0));
    AITimeMs.setPosition({BOARD_SIZE_WITH_PADDING, DisplayService::WINDOW_HEIGHT - PADDING - 20});

    window.draw(AITimeMs);

    sf::Text depthLiveText(getSharedFont(),
                      "AI Depth: " + std::to_string(depthLive) + " | Nodes visited: " + std::to_string(nodesVisited));
    depthLiveText.setCharacterSize(18);
    depthLiveText.setFillColor(sf::Color(0, 150, 0));
    depthLiveText.setPosition({BOARD_SIZE_WITH_PADDING, DisplayService::WINDOW_HEIGHT - PADDING - 40});

    window.draw(depthLiveText);
}

bool PvEScene::handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    int captures[8] = {};
    int count = 0;
    if (!event || !event->is<sf::Event::MouseButtonPressed>())
    {
        return false;
    }

    if (const auto& mousePressedEvent = event->getIf<sf::Event::MouseButtonPressed>(); mousePressedEvent &&
        mousePressedEvent->button == sf::Mouse::Button::Left)
    {
        const auto& mousePos = mousePressedEvent->position;
        const auto [row, col] = getCellFromMousePosition(mousePos);
        illegalMove = getLegalMove(row, col);
        if (illegalMove != IllegalMoves::Type::NONE)
        {
            return false;
        }
        const auto playerMove = Position(row, col);

        playMove(playerMove);
        draw(window);
        if (CheckWinService::isWin(board))
        {
            return true;
        }

        json decisionTree = json::array();
        moveHistory.push_back(playerMove);
        const auto aiMove = handleAITurn(&depthLive, &nodesVisited);
        playMove(aiMove);
        if (CaptureService::checkCapture(board, Board::getGlobalIndex(aiMove), colorToPlay == sf::Color::White,
                                         captures, count) != 0)
        {
            if (captureSound)
            {
                captureSound->play();
            }
        }
        return true;
    }
    return false;
}

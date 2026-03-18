#include "PvEScene.hpp"
#include "CheckWinService.hpp"
#include "SFML/Graphics/Text.hpp"
#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"
#include "DisplayService.hpp"
#include "HeuristicService.hpp"
#include "utils/getSharedFont.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

void PvEScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (winningColor) return;
    winningColor = handleStonePlacement(event, window);
    if (winningColor && *winningColor == sf::Color::White && loseSound) {
        loseSound->play();
    }
    else if (winningColor && winSound) {
        winSound->play();
    }
}

void PvEScene::drawTexts(sf::RenderWindow& window) {
    sf::Text AITimeMs(getSharedFont(),
                      "Time taken : " + (colorToPlay == sf::Color::Black
                                             ? std::to_string(lastAITimeMs) + " ms"
                                             : "playing..."));
    AITimeMs.setCharacterSize(18);
    AITimeMs.setFillColor(lastAITimeMs >= 500 ? sf::Color::Red : sf::Color(0, 150, 0));
    AITimeMs.setPosition({BOARD_SIZE_WITH_PADDING, DisplayService::WINDOW_HEIGHT - PADDING - 20});

    window.draw(AITimeMs);

    sf::Text depthLiveText(getSharedFont(),
                           "AI Depth: " + std::to_string(depthLive) + " | Nodes visited: " + std::to_string(
                               nodesVisited));
    depthLiveText.setCharacterSize(18);
    depthLiveText.setFillColor(sf::Color(0, 150, 0));
    depthLiveText.setPosition({BOARD_SIZE_WITH_PADDING, DisplayService::WINDOW_HEIGHT - PADDING - 40});

    window.draw(depthLiveText);
}

const sf::Color *PvEScene::handleStonePlacement(const std::optional<sf::Event> &event, sf::RenderWindow &window) {
    if (!event || !event->is<sf::Event::MouseButtonPressed>()) {
        return nullptr;
    }

    if (const auto& mousePressedEvent = event->getIf<sf::Event::MouseButtonPressed>(); mousePressedEvent &&
        mousePressedEvent->button == sf::Mouse::Button::Left) {
        const auto& mousePos = mousePressedEvent->position;
        const auto [row, col] = getCellFromMousePosition(mousePos);
        illegalMove = getLegalMove(row, col);
        if (illegalMove != IllegalMoves::Type::NONE) {
            return nullptr;
        }
        Position playerMove = {row, col};

        playMove(playerMove);
        if (winningColor) {
            return winningColor;
        }
        draw(window);
        winningColor = CheckWinService::isWin(board);
        if (winningColor) {
            return winningColor;
        }

        moveHistory.push_back(playerMove);
        const auto aiMove = handleAITurn(&depthLive, &nodesVisited);
        playMove(aiMove);
        if (winningColor) {
            return winningColor;
        }
        return CheckWinService::isWin(board);
    }
    return nullptr;
}

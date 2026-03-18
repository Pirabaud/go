#include "PvPScene.hpp"
#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"
#include "CheckWinService.hpp"

void PvPScene::handleEvent(const std::optional<sf::Event> &event, sf::RenderWindow &window) {
    if (winningColor) return;
    if (handleHistoryEvent(event, window)) {
        needsAiSuggestion = true;
        aiSuggestionTimer.restart();
        draw(window);
    }
    winningColor = handleStonePlacement(event, window);
    if (winningColor && winSound) {
        winSound->play();
    }
    draw(window);
    if (!winningColor) {
        needsAiSuggestion = true;
        aiSuggestionTimer.restart();
    }
}

void PvPScene::drawTexts(sf::RenderWindow &window) {
    if (needsAiSuggestion && aiSuggestionTimer.getElapsedTime().asMilliseconds() > 300) {
        this->suggestedMove = handleAITurn();
        needsAiSuggestion = false;
    }
}

const sf::Color *PvPScene::handleStonePlacement(const std::optional<sf::Event> &event, sf::RenderWindow &window) {
    if (!event || !event->is<sf::Event::MouseButtonPressed>()) {
        return nullptr;
    }

    if (const auto &mousePressedEvent = event->getIf<sf::Event::MouseButtonPressed>(); mousePressedEvent &&
        mousePressedEvent->button == sf::Mouse::Button::Left) {
        const auto &mousePos = mousePressedEvent->position;
        const auto [row, col] = getCellFromMousePosition(mousePos);
        illegalMove = getLegalMove(row, col);
        if (illegalMove != IllegalMoves::Type::NONE) {
            return nullptr;
        }
        playMove(Position{row, col});
        if (winningColor) {
            return winningColor;
        }
        draw(window);
        return CheckWinService::isWin(board);
    }
    return nullptr;
}

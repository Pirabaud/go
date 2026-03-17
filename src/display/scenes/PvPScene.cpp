#include "PvPScene.hpp"
#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"
#include "CheckWinService.hpp"

void PvPScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (winningColor) return;
    if (handleHistoryEvent(event, window)) {
        needsAiSuggestion = true;
        aiSuggestionTimer.restart();
        draw(window);
    }
    const bool doesStoneHaveBeenPlaced = handleStonePlacement(event, window);
    if (doesStoneHaveBeenPlaced) {
        winningColor = CheckWinService::isWin(board);
        if (winningColor && winSound) {
            winSound->play();
        }
        draw(window);
        if (!winningColor) {
            needsAiSuggestion = true;
            aiSuggestionTimer.restart();
        }
        draw(window);
        if (!winningColor) {
            this->suggestedMove = handleAITurn();
        }
    }
}

void PvPScene::drawTexts(sf::RenderWindow& window) {
    if (needsAiSuggestion && aiSuggestionTimer.getElapsedTime().asMilliseconds() > 300) {
        this->suggestedMove = handleAITurn();
        needsAiSuggestion = false;
    }
}

bool PvPScene::handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    int captures[8] = {};
    int count = 0;
    if (!event || !event->is<sf::Event::MouseButtonPressed>()) {
        return false;
    }

    if (const auto& mousePressedEvent = event->getIf<sf::Event::MouseButtonPressed>(); mousePressedEvent &&
        mousePressedEvent->button == sf::Mouse::Button::Left) {
        const auto& mousePos = mousePressedEvent->position;
        const auto [row, col] = getCellFromMousePosition(mousePos);
        illegalMove = getLegalMove(row, col);
        if (illegalMove != IllegalMoves::Type::NONE) {
            return false;
        }
        playMove(Position{row, col});
        return true;
    }
    return false;
}

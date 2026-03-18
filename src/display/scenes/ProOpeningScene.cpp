#include "ProOpeningScene.hpp"
#include <iostream>
#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"
#include "CheckWinService.hpp"

void ProOpeningScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
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

void ProOpeningScene::drawTexts(sf::RenderWindow& window) {
    if (needsAiSuggestion && aiSuggestionTimer.getElapsedTime().asMilliseconds() > 300 && moveNumber > 2) {
        this->suggestedMove = handleAITurn();
        needsAiSuggestion = false;
    }
}

bool ProOpeningScene::isOutsideProZone(int row, int col) {
    const int center = Board::SIZE / 2;
    const int distRow = std::abs(row - center);
    const int distCol = std::abs(col - center);
    return std::max(distRow, distCol) >= 3;
}

const sf::Color *ProOpeningScene::handleStonePlacement(const std::optional<sf::Event> &event, sf::RenderWindow &window) {
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
        if (moveNumber == 0 && (row != Board::SIZE / 2 || col != Board::SIZE / 2)) {
            illegalMove = IllegalMoves::Type::PRO_OPENING_FIRST_IN_MIDDLE;
            return nullptr;
        }
        if (moveNumber == 2 && !isOutsideProZone(row, col)) {
            illegalMove = IllegalMoves::Type::PRO_OPENING_THIRD_AT_DISTANCE;
            return nullptr;
        }
        playMove(Position{row, col});
        if (winningColor) {
            return winningColor;
        }
        draw(window);
        moveNumber++;
        return CheckWinService::isWin(board);
    }
    return nullptr;
}

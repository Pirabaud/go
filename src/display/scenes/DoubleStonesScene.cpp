#include "DoubleStonesScene.hpp"
#include "CheckWinService.hpp"
#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"
#include "DisplayService.hpp"

void DoubleStonesScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (winningColor) return;

    const auto* keyPressedEvent = event->getIf<sf::Event::KeyPressed>();
    if (keyPressedEvent && keyPressedEvent->code == sf::Keyboard::Key::Left) {
        if (!pastMoves.empty()) {
            moveNumber--;
        }
    }
    else if (keyPressedEvent && keyPressedEvent->code == sf::Keyboard::Key::Right) {
        if (!futureMoves.empty()) {
            moveNumber++;
        }
    }
    if (handleHistoryEvent(event, window)) {
        colorToPlay = ((moveNumber / 2) % 2 == 0) ? sf::Color::Black : sf::Color::White;
    }

    winningColor = handleStonePlacement(event, window);
        if (winningColor && winSound) {
            winSound->play();
        }
}

void DoubleStonesScene::drawTexts(sf::RenderWindow& window) {
}

const sf::Color *DoubleStonesScene::handleStonePlacement(const std::optional<sf::Event> &event,
                                                         sf::RenderWindow &window) {
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
        moveNumber++;
        if (moveNumber % 2 != 0) {
            nextTurn();
        }
        draw(window);
        return CheckWinService::isWin(board);
    }
    return nullptr;
}

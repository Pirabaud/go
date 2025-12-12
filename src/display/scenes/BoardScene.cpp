#include "BoardScene.hpp"

#include <iostream>

#include "CaptureService.hpp"
#include "DisplayService.hpp"
#include "MinMax.hpp"
#include "SFML/Graphics/CircleShape.hpp"

float BoardScene::PADDING = 40.0f;
float BoardScene::BOARD_SIZE = DisplayService::WINDOW_HEIGHT - 2 * PADDING; // 200 is the width of the sidebar
float BoardScene::BOARD_SIZE_WITH_PADDING = BOARD_SIZE + 2 * PADDING;
float BoardScene::CELL_SIZE = BOARD_SIZE / (Board::SIZE - 1);
float BoardScene::STONE_RADIUS = CELL_SIZE / 2.0f - 2.0f;

BoardScene::BoardScene(sf::RenderWindow& window) {
    backgroundColor = sf::Color(206, 163, 70);
}

void BoardScene::draw(sf::RenderWindow& window) {
    window.clear(backgroundColor);
    drawBoard(window);
    drawStones(window);
    drawTexts(window);
    window.display();
}

void BoardScene::drawBoard(sf::RenderWindow& window) {
    sf::Vertex line[2];

    for (int i = 0; i < Board::SIZE; ++i) {
        const float y = PADDING + i * CELL_SIZE;
        line[0] = sf::Vertex{sf::Vector2f(PADDING, y), sf::Color::Black};
        line[1] = sf::Vertex{sf::Vector2f(PADDING + CELL_SIZE * (Board::SIZE - 1), y), sf::Color::Black};
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    for (int i = 0; i < Board::SIZE; ++i) {
        const float x = PADDING + i * CELL_SIZE;
        line[0] = sf::Vertex{sf::Vector2f(x, PADDING), sf::Color::Black};
        line[1] = sf::Vertex{sf::Vector2f(x, PADDING + CELL_SIZE * (Board::SIZE - 1)), sf::Color::Black};
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

void BoardScene::drawStones(sf::RenderWindow& window) {
    drawSingleColorStone(board.getBitBoardWhite(), window, sf::Color::White);
    drawSingleColorStone(board.getBitBoardBlack(), window, sf::Color::Black);
}

void BoardScene::drawSingleColorStone(const std::array<uint64_t, 6>& bitBoard, sf::RenderWindow& window,
                                      const sf::Color& color) {
    for (int row = 0; row < Board::SIZE; ++row) {
        for (int col = 0; col < Board::SIZE; ++col) {
            if (Board::isBitAt(bitBoard, Board::getGlobalIndex({row, col}))) {
                // Draw a stone at the position
                const float x = PADDING + col * CELL_SIZE;
                const float y = PADDING + row * CELL_SIZE;
                sf::CircleShape stone(CELL_SIZE / 2.0f - 2.0f);
                stone.setFillColor(color);
                stone.setPosition({x - stone.getRadius(), y - stone.getRadius()});
                window.draw(stone);
            }
        }
    }
}

std::pair<int, int> BoardScene::getCellFromMousePosition(const sf::Vector2i& mousePos) const {
    const auto fx = static_cast<float>(mousePos.x);
    const auto fy = static_cast<float>(mousePos.y);

    for (int row = 0; row < Board::SIZE; ++row) {
        for (int col = 0; col < Board::SIZE; ++col) {
            const float cx = PADDING + col * CELL_SIZE;
            const float cy = PADDING + row * CELL_SIZE;

            const float dx = fx - cx;
            const float dy = fy - cy;
            const float distSq = dx * dx + dy * dy;

            if (distSq <= STONE_RADIUS * STONE_RADIUS) {
                return {row, col};
            }
        }
    }
    return {-1, -1}; // Return an invalid position if no stone was placed
}

void BoardScene::handleAITurn(Position playerMove, json& decisionTree,  std::vector<Position>& moveHistory) {
    if (colorToPlay == sf::Color::White) { // L'IA joue les blancs
        const MinMax ai(board);
        auto [aiMove, elapsedTimeMS] = ai.run(playerMove, decisionTree, moveHistory);
        lastAITimeMs = elapsedTimeMS;
        moveHistory.push_back(aiMove);
        if (aiMove.x != -1 && aiMove.y != -1) {
            playMove(aiMove);
        }
        CaptureService::checkCapture(board.getBitBoardWhite(), board.getBitBoardBlack(), aiMove);
    }
}

void BoardScene::playMove(Position pos) {
    colorToPlay == sf::Color::White ? board.addStoneWhite(pos) : board.addStoneBlack(pos);
    nextTurn();
}

void BoardScene::nextTurn() {
    colorToPlay == sf::Color::White ? colorToPlay = sf::Color::Black : colorToPlay = sf::Color::White;
}

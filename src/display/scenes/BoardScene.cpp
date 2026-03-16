#include "BoardScene.hpp"

#include <iostream>

#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"
#include "CheckWinService.hpp"
#include "DisplayService.hpp"
#include "MinMax.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Audio.hpp"

float BoardScene::PADDING = 40.0f;
float BoardScene::BOARD_SIZE = DisplayService::WINDOW_HEIGHT - 2 * PADDING;
float BoardScene::BOARD_SIZE_WITH_PADDING = BOARD_SIZE + 2 * PADDING;
float BoardScene::CELL_SIZE = BOARD_SIZE / (Board::SIZE - 1);
float BoardScene::STONE_RADIUS = CELL_SIZE / 2.0f - 2.0f;

BoardScene::BoardScene(sf::RenderWindow& window) {
    backgroundColor = sf::Color(206, 163, 70);
    _loadSound("../assets/move.mp3", placeStoneSoundBuffer, placeStoneSound);
    _loadSound("../assets/capture.mp3", captureSoundBuffer, captureSound);
    _loadSound("../assets/illegal.mp3", illegalMoveSoundBuffer, illegalMoveSound);
    _loadSound("../assets/win.mp3", winSoundBuffer, winSound);
    _loadSound("../assets/lose.mp3", loseSoundBuffer, loseSound);
}

void BoardScene::_loadSound(const std::string& filePath, sf::SoundBuffer*& buffer, sf::Sound*& sound) {
    buffer = new sf::SoundBuffer();
    if (!buffer->loadFromFile(filePath)) {
        std::cerr << "Failed to load sound effect from " << filePath << "!" << std::endl;
        delete buffer;
        buffer = nullptr;
        sound = nullptr;
    } else {
        sound = new sf::Sound(*buffer);
    }
}

BoardScene::~BoardScene() {
    delete placeStoneSoundBuffer;
    delete placeStoneSound;
    delete captureSoundBuffer;
    delete captureSound;
    delete illegalMoveSoundBuffer;
    delete illegalMoveSound;
    delete winSoundBuffer;
    delete winSound;
    delete loseSoundBuffer;
    delete loseSound;
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
    if (suggestedMove.x != -1 && suggestedMove.y != -1) {
        drawSuggestedMove(window);
    }
}

void BoardScene::drawSuggestedMove(sf::RenderWindow& window) const {
    const float x = BoardScene::PADDING + suggestedMove.y * BoardScene::CELL_SIZE;
    const float y = BoardScene::PADDING + suggestedMove.x * BoardScene::CELL_SIZE;
    sf::CircleShape suggestion(BoardScene::STONE_RADIUS);
    suggestion.setFillColor(sf::Color(255, 0, 0, 128)); // Red with some transparency
    suggestion.setPosition({x - suggestion.getRadius(), y - suggestion.getRadius()});
    window.draw(suggestion);
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

Position BoardScene::handleAITurn() {
        MinMax ai(board);
        // auto [aiMove, elapsedTimeMS] = ai.run(playerMove, decisionTree, moveHistory);
        auto [aiMove, elapsedTimeMS] = ai.run(500, colorToPlay != sf::Color::White); // 5 secondes de temps de réflexion
        lastAITimeMs = elapsedTimeMS;
        return aiMove;
}

void BoardScene::playMove(Position pos) {
    this->suggestedMove = {-1, -1};
    if (this->placeStoneSound)
    {
        this->placeStoneSound->play();
    }
    colorToPlay == sf::Color::White ? board.addStoneWhite(pos) : board.addStoneBlack(pos);
    nextTurn();
}

void BoardScene::nextTurn() {
    colorToPlay == sf::Color::White ? colorToPlay = sf::Color::Black : colorToPlay = sf::Color::White;
}

IllegalMoves::Type BoardScene::getLegalMove(const int row,const int col)
{
    IllegalMoves::Type legalMove = IllegalMoves::NONE;
    legalMove = CheckLegalMove::isLegalMove(Board::getGlobalIndex({row, col}), board, colorToPlay == sf::Color::Black);
    if (legalMove != IllegalMoves::Type::NONE && this->illegalMoveSound){
        this->illegalMoveSound->play();
    }
    return legalMove;
}

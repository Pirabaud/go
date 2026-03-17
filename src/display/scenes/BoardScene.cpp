#include "BoardScene.hpp"

#include <iostream>

#include "CaptureService.hpp"
#include "CheckLegalMove.hpp"
#include "CheckWinService.hpp"
#include "DisplayService.hpp"
#include "getSharedFont.hpp"
#include "loadSound.hpp"
#include "MinMax.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

float BoardScene::PADDING = 40.0f;
float BoardScene::BOARD_SIZE = DisplayService::WINDOW_HEIGHT - 2 * PADDING;
float BoardScene::BOARD_SIZE_WITH_PADDING = BOARD_SIZE + 2 * PADDING;
float BoardScene::CELL_SIZE = BOARD_SIZE / (Board::SIZE - 1);
float BoardScene::STONE_RADIUS = CELL_SIZE / 2.0f - 2.0f;

void BoardScene::setBoardVars()
{
    PADDING = 40.0f;
    BOARD_SIZE = DisplayService::WINDOW_HEIGHT - 2 * PADDING;
    CELL_SIZE = BOARD_SIZE / (Board::SIZE - 1);
    BOARD_SIZE_WITH_PADDING = BOARD_SIZE + 2 * PADDING;
    STONE_RADIUS = CELL_SIZE / 2.0f - 2.0f;
}

BoardScene::BoardScene(sf::RenderWindow& window) {
    backgroundColor = sf::Color(206, 163, 70);
    loadSound("../assets/move.mp3", placeStoneSoundBuffer, placeStoneSound);
    loadSound("../assets/capture.mp3", captureSoundBuffer, captureSound);
    loadSound("../assets/illegal.mp3", illegalMoveSoundBuffer, illegalMoveSound);
    loadSound("../assets/win.mp3", winSoundBuffer, winSound);
    loadSound("../assets/lose.mp3", loseSoundBuffer, loseSound);
    loadSound("../assets/history.mp3", historySoundBuffer, historySound);
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
    drawDefaultTexts(window);
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

void BoardScene::drawDefaultTexts(sf::RenderWindow& window) const
{

    sf::Text blackCapturesText(getSharedFont(),
                               "Black captures: " + std::to_string(board.getWhiteCaptured()));
    blackCapturesText.setCharacterSize(18);
    blackCapturesText.setFillColor(sf::Color::Black);
    blackCapturesText.setPosition({BOARD_SIZE_WITH_PADDING, PADDING + 20});
    window.draw(blackCapturesText);
    sf::Text whiteCapturesText(getSharedFont(),
                               "White captures: " + std::to_string(board.getBlackCaptured()));
    whiteCapturesText.setCharacterSize(18);
    whiteCapturesText.setFillColor(sf::Color::White);
    whiteCapturesText.setPosition({BOARD_SIZE_WITH_PADDING, PADDING + 40});
    window.draw(whiteCapturesText);
    if (illegalMove != IllegalMoves::Type::NONE)
    {
        sf::Text illegalMoveText(getSharedFont(),
                                 "This is move is illegal because " + std::string(IllegalMoves::toString(illegalMove)) +
                                 ".");
        illegalMoveText.setCharacterSize(18);
        illegalMoveText.setFillColor(sf::Color::Red);
        illegalMoveText.setPosition({BOARD_SIZE_WITH_PADDING, PADDING + 60});

        window.draw(illegalMoveText);
    }

    sf::CircleShape stoneToPlay(20);
    stoneToPlay.setFillColor(colorToPlay);
    stoneToPlay.setPosition({DisplayService::WINDOW_WIDTH - PADDING - 20, DisplayService::WINDOW_HEIGHT - PADDING - 20});
    window.draw(stoneToPlay);

    if (winningColor) {
        sf::Text winText(getSharedFont(),
                         "Player " + std::string(*winningColor == sf::Color::White ? "White" : "Black") + " wins!");

        winText.setCharacterSize(30);
        winText.setFillColor(sf::Color::White);
        winText.setPosition({static_cast<float>(DisplayService::WINDOW_WIDTH / 2 - 100), static_cast<float>(DisplayService::WINDOW_HEIGHT / 2 - 50)});

        sf::Text newGameText(getSharedFont(),
                         "Press ESC to go to main menu.");

        newGameText.setCharacterSize(18);
        newGameText.setFillColor(sf::Color::White);
        newGameText.setPosition({static_cast<float>(DisplayService::WINDOW_WIDTH / 2 - 110), static_cast<float>(DisplayService::WINDOW_HEIGHT / 2)});

        sf::RectangleShape fadeBackground(sf::Vector2f(DisplayService::WINDOW_WIDTH, DisplayService::WINDOW_HEIGHT));
        fadeBackground.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
        fadeBackground.setPosition({0, 0});

        window.draw(fadeBackground);
        window.draw(winText);
        window.draw(newGameText);
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

bool BoardScene::handleHistoryEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    const auto* keyPressedEvent = event->getIf<sf::Event::KeyPressed>();
    if (!keyPressedEvent) return false;
    if (keyPressedEvent->code == sf::Keyboard::Key::Left)
    {
        if (pastMoves.empty())
        {
            if (illegalMoveSound)
            {
                illegalMoveSound->play();
            }
            return false;
        }
        suggestedMove = {-1, -1};
        auto lastMove = pastMoves.top();
        colorToPlay = lastMove.isBlack ? sf::Color::Black : sf::Color::White;
        lastMove.isBlack ? board.removeBlackStone(Board::getGlobalIndex(lastMove.pos)) : board.removeWhiteStone(Board::getGlobalIndex(lastMove.pos));
        for (const auto& capturedIndex : lastMove.capturedIndices)
        {
            if (capturedIndex == -1) continue;
            lastMove.isBlack ? board.addStoneWhite(capturedIndex) :
                board.addStoneBlack(capturedIndex);
            board.addCaptures(lastMove.isBlack,-1);
        }
        pastMoves.pop();
        futureMoves.push(lastMove);
        if (historySound)
        {
            historySound->setPitch(.7f);
            historySound->play();
        }
        return true;
    }
    if (keyPressedEvent->code == sf::Keyboard::Key::Right)
    {
        if (futureMoves.empty()) {
            if (illegalMoveSound)
            {
                illegalMoveSound->play();
            }
            return false;
        };
        suggestedMove = {-1, -1};
        auto nextMove = futureMoves.top();
        colorToPlay = nextMove.isBlack ? sf::Color::White : sf::Color::Black;
        nextMove.isBlack ? board.addStoneBlack(nextMove.pos) : board.addStoneWhite(nextMove.pos);
        for (const auto& capturedIndex : nextMove.capturedIndices)
        {
            if (capturedIndex == -1) continue;
            nextMove.isBlack ? board.removeWhiteStone(capturedIndex) :
                board.removeBlackStone(capturedIndex);
            board.addCaptures(nextMove.isBlack,1);

        }
        futureMoves.pop();
        pastMoves.push(nextMove);

        if (historySound)
        {
            historySound->setPitch(.9f);
            historySound->play();
        }
        return true;
    }
    return false;
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
    int captures[8] = {-1};
    int count = 0;
    futureMoves = std::stack<MoveInfo>();
    this->suggestedMove = {-1, -1};
    if (this->placeStoneSound)
    {
        this->placeStoneSound->play();
    }
    if (CaptureService::checkCapture(board, Board::getGlobalIndex(pos), colorToPlay == sf::Color::Black, captures, count))
    {
        if (captureSound)
        {
            captureSound->play();
        }
    }
    MoveInfo historicalMove = {.pos = pos, .isBlack = colorToPlay == sf::Color::Black, .capturedIndices = {}};
    for (int i = 0; i < count; ++i)
    {
        if (captures[i] != -1)
        {
            historicalMove.capturedIndices.push_back(captures[i]);
        }
    }
    pastMoves.push(historicalMove);
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

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
    }
}

void PvEScene::drawTexts(sf::RenderWindow& window)
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
    sf::Text AITimeMs(getSharedFont(),
                      "Time taken : " + (colorToPlay == sf::Color::Black
                                             ? std::to_string(lastAITimeMs) + " ms"
                                             : "playing..."));
    AITimeMs.setCharacterSize(18);
    AITimeMs.setFillColor(lastAITimeMs >= 500 ? sf::Color::Red : sf::Color(0, 150, 0));
    AITimeMs.setPosition({BOARD_SIZE_WITH_PADDING, DisplayService::WINDOW_HEIGHT - PADDING - 20});

    window.draw(AITimeMs);


    if (winningColor)
    {
        if (*winningColor == sf::Color::Black && winSound)
        {
            winSound->play();
        }
        else if (loseSound)
        {
            loseSound->play();
        }
        sf::Text winText(getSharedFont(),
                         "Player " + std::string(*winningColor == sf::Color::White ? "White" : "Black") + " wins!");

        winText.setCharacterSize(30);
        winText.setFillColor(sf::Color::White);
        winText.setPosition({
            static_cast<float>(DisplayService::WINDOW_WIDTH / 2 - 100),
            static_cast<float>(DisplayService::WINDOW_HEIGHT / 2 - 50)
        });

        sf::Text newGameText(getSharedFont(),
                             "Press ESC to go to main menu.");

        newGameText.setCharacterSize(18);
        newGameText.setFillColor(sf::Color::White);
        newGameText.setPosition({
            static_cast<float>(DisplayService::WINDOW_WIDTH / 2 - 110),
            static_cast<float>(DisplayService::WINDOW_HEIGHT / 2)
        });

        sf::RectangleShape fadeBackground(sf::Vector2f(DisplayService::WINDOW_WIDTH, DisplayService::WINDOW_HEIGHT));
        fadeBackground.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
        fadeBackground.setPosition({0, 0});

        window.draw(fadeBackground);
        window.draw(winText);
        window.draw(newGameText);
    }
}

bool PvEScene::handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    int captures[8];
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
        auto playerMove = Position(row, col);

        playMove(playerMove);

        if (CaptureService::checkCapture(board, Board::getGlobalIndex(playerMove), true, captures, count) != 0)
        {
            if (captureSound)
            {
                captureSound->play();
            }
        }
        draw(window);
        if (CheckWinService::isWin(board))
        {
            return true;
        }

        json decisionTree = json::array();
        moveHistory.push_back(playerMove);
        const auto aiMove = handleAITurn();
        playMove(aiMove);
        if (CaptureService::checkCapture(board, Board::getGlobalIndex(aiMove), colorToPlay == sf::Color::White,
                                         captures, count) != 0)
        {
            if (captureSound)
            {
                captureSound->play();
            }
        }
        //this->suggestedMove = handleAITurn();
        return true;
    }
    return false;
}

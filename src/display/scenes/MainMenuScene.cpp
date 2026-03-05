#include "MainMenuScene.hpp"

#include "BoardScene.hpp"
#include "DisplayService.hpp"
#include "DoubleStonesScene.hpp"
#include "PvEScene.hpp"
#include "PvPScene.hpp"

#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define BUTTON_GAP 10

float getButtonXPosition() {
    return static_cast<float>(DisplayService::WINDOW_WIDTH / 2 - BUTTON_WIDTH / 2);
}

float getTotalHeightButtons(int numberOfButtons) {
    return static_cast<float>(numberOfButtons * BUTTON_HEIGHT + (numberOfButtons - 1) * BUTTON_GAP);
}

float getButtonYPosition(int buttonIndex) {
    return DisplayService::WINDOW_HEIGHT / 2 - getTotalHeightButtons(5) / 2 + buttonIndex * (BUTTON_HEIGHT + BUTTON_GAP);
}

MainMenuScene::MainMenuScene(sf::RenderWindow& win):
    pvpButton({BUTTON_WIDTH, BUTTON_HEIGHT}, {getButtonXPosition(), getButtonYPosition(0)}, "PvP", [this, &win]()
    {
        DisplayService::changeScene(new PvPScene(win));
    }),
    pveButton({BUTTON_WIDTH, BUTTON_HEIGHT}, {getButtonXPosition(), getButtonYPosition(1)}, "PvE", [this, &win]()
    {
        DisplayService::changeScene(new PvEScene(win));
    }),
    doubleStonesButton({BUTTON_WIDTH, BUTTON_HEIGHT}, {getButtonXPosition(), getButtonYPosition(2)}, "Double Stones", [this, &win]()
    {
        DisplayService::changeScene(new DoubleStonesScene(win));
    }),
    proStartButton({BUTTON_WIDTH, BUTTON_HEIGHT}, {getButtonXPosition(), getButtonYPosition(3)}, "Pro start", [this, &win]()
    {
        DisplayService::changeScene(new PvEScene(win));
    }),
    swapStartButton({BUTTON_WIDTH, BUTTON_HEIGHT}, {getButtonXPosition(), getButtonYPosition(4)}, "Swap start", [this, &win]()
    {
        DisplayService::changeScene(new PvEScene(win));
    })

{
    backgroundColor = sf::Color(200, 200, 200);
}

void MainMenuScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)

{
    if (event->is<sf::Event::Closed>())
    {
        window.close();
    }
    pvpButton.handleEvent(event, window);
    pveButton.handleEvent(event, window);
        doubleStonesButton.handleEvent(event, window);
        proStartButton.handleEvent(event, window);
        swapStartButton.handleEvent(event, window);
}

void MainMenuScene::draw(sf::RenderWindow& window)
{
    window.clear(backgroundColor);
    pvpButton.draw(window);
    pveButton.draw(window);
        doubleStonesButton.draw(window);
        proStartButton.draw(window);
        swapStartButton.draw(window);
    window.display();
}


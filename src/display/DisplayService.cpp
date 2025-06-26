#include "DisplayService.hpp"

#include <iostream>

#include "BoardScene.hpp"
#include "MainMenuScene.hpp"
#include "SFML/Window/VideoMode.hpp"

unsigned DisplayService::WINDOW_WIDTH = 1400;
unsigned DisplayService::WINDOW_HEIGHT = 800;
AbstractScene* DisplayService::selectedScene = nullptr;

DisplayService::DisplayService() {
}

DisplayService::~DisplayService() {
    delete selectedScene;
}

void DisplayService::start() {
    if (isInitialized || window.isOpen()) return;
    window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Gomoku");
    isInitialized = true;
    changeScene(new MainMenuScene(window));
    while (window.isOpen()) {
        if (selectedScene) {
            selectedScene->draw(window);
        }
        while (const std::optional event = window.pollEvent()) {
            handleEvent(event);
            handleKeyboardEvents(window);
            if (selectedScene) {
                selectedScene->handleEvent(event, window);
            }
        }
    }
}

void DisplayService::changeScene(AbstractScene* scene) {
    delete selectedScene;
    selectedScene = scene;
}

void DisplayService::handleEvent(const std::optional<sf::Event>& event) {
    if (!event.has_value()) return;

    if (event->is<sf::Event::Closed>()) {
        window.close();
    }

    if (event->is<sf::Event::Resized>()) {
        const auto& eventSize = event->getIf<sf::Event::Resized>();
        if (!eventSize) return;
        const sf::FloatRect visibleArea(
            {0, 0}, {static_cast<float>(eventSize->size.x), static_cast<float>(eventSize->size.y)});
        window.setView(sf::View(visibleArea));
    }
}

void DisplayService::handleKeyboardEvents(sf::RenderWindow& window) const {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Numpad0)) {
        changeScene(new MainMenuScene(window));
    }
}

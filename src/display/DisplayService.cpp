#include "DisplayService.hpp"

#include <iostream>

#include "MainMenuScene.hpp"
#include "SFML/Window/VideoMode.hpp"

unsigned DisplayService::WINDOW_WIDTH = 800;
unsigned DisplayService::WINDOW_HEIGHT = 800;
AbstractScene* DisplayService::selectedScene = nullptr;

DisplayService::~DisplayService()
{
    delete selectedScene;
}

void DisplayService::start()
{
    if (isInitialized || window.isOpen()) return;
    window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Gomoku");
    isInitialized = true;
    changeScene(new MainMenuScene(window));
    while (window.isOpen())
    {
        if (selectedScene)
        {
            selectedScene->draw(window);
        }
        while (const std::optional event = window.pollEvent())
        {
            handleEvent(event);
            if (selectedScene)
            {
                selectedScene->handleEvent(event, window);
            }
        }
    }
}

void DisplayService::changeScene(AbstractScene* scene)
{
    delete selectedScene;
    selectedScene = scene;
}

void DisplayService::handleEvent(const std::optional<sf::Event>& event)
{
    if (!event.has_value()) return;

    if (event->is<sf::Event::Closed>())
    {
        window.close();
    }
}

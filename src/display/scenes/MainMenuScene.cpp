#include "MainMenuScene.hpp"

#include "BoardScene.hpp"
#include "DisplayService.hpp"
#include "PvEScene.hpp"
#include "PvPScene.hpp"

MainMenuScene::MainMenuScene(sf::RenderWindow& win):
    pvpButton({200, 50}, {300, 100}, "PvP", [this, &win]()
    {
        DisplayService::changeScene(new PvPScene(win));
    }),
    pveButton({200, 50}, {300, 200}, "PvE", [this, &win]()
    {
        DisplayService::changeScene(new PvEScene(win));
    }),
    simulationButton({200, 50}, {300, 300}, "Simulation Test", [this]()
    {
        backgroundColor = sf::Color::Red;
    })

{
}

void MainMenuScene::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)

{
    if (event->is<sf::Event::Closed>())
    {
        window.close();
    }
    pvpButton.handleEvent(event, window);
    pveButton.handleEvent(event, window);
    simulationButton.handleEvent(event, window);
}

void MainMenuScene::draw(sf::RenderWindow& window)
{
    window.clear(backgroundColor);
    pvpButton.draw(window);
    pveButton.draw(window);
    simulationButton.draw(window);
    window.display();
}

void MainMenuScene::Ai(sf::RenderWindow &window) {
}

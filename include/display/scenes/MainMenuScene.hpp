#ifndef MAIN_MENU_SCENE_HPP
#define MAIN_MENU_SCENE_HPP
#include <iostream>

#include "AbstractScene.hpp"
#include "Button.hpp"


class MainMenuScene final : public AbstractScene
{
private:
    Button pvpButton;
    Button pveButton;
    Button simulationButton;

public:
    explicit MainMenuScene(sf::RenderWindow& win);

    void handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};


#endif //MAIN_MENU_SCENE_HPP

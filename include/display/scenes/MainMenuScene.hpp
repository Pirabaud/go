#ifndef MAIN_MENU_SCENE_HPP
#define MAIN_MENU_SCENE_HPP

#include "AbstractScene.hpp"
#include "Button.hpp"


class MainMenuScene final : public AbstractScene {
public:
    explicit MainMenuScene(sf::RenderWindow& win);

    void handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

private:
    Button pvpButton;
    Button pveButton;
    Button doubleStonesButton;
    Button proStartButton;
    Button otherSizeButton;
};


#endif //MAIN_MENU_SCENE_HPP

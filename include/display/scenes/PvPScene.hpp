//
// Created by afaby on 6/20/25.
//

#ifndef PVP_SCENE_HPP
#define PVP_SCENE_HPP
#include "BoardScene.hpp"
#include "Button.hpp"
#include "DisplayService.hpp"
#include "MainMenuScene.hpp"


class PvPScene : public BoardScene
{
public:
   explicit  PvPScene(sf::RenderWindow& window)
        : BoardScene(window)
    {
        backgroundColor = sf::Color(206, 163, 70);
    }

    void handleEvent(const std::optional<sf::Event>&, sf::RenderWindow& window) override;
    void drawTexts(sf::RenderWindow& window) override;
    bool handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

private:
    const sf::Color* winningColor = nullptr;
};


#endif //PVP_SCENE_HPP

//
// Created by Pierre Rabaud on 06/10/2025.
//

#ifndef PVE_SCENE_HPP
#define PVE_SCENE_HPP
#include "AiPlay.hpp"
#include "BoardScene.hpp"

class PvEScene : public BoardScene {
public:
    explicit PvEScene(sf::RenderWindow& window)
        : BoardScene(window) {
        backgroundColor = sf::Color(135, 206, 235);
    }
    void handleEvent(const std::optional<sf::Event>&, sf::RenderWindow& window) override;
    void drawTexts(sf::RenderWindow& window) override;
    void Ai(sf::RenderWindow& window) override;
    bool handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

private:
    const sf::Color* winningColor = nullptr;
    bool playerPlay = false;
    bool AIPlay();
};

#endif
#ifndef DOUBLE_STONES_SCENE_HPP
#define DOUBLE_STONES_SCENE_HPP
#include "BoardScene.hpp"

class DoubleStonesScene : public BoardScene {
public:
    explicit DoubleStonesScene(sf::RenderWindow& window)
        : BoardScene(window) {
        backgroundColor = sf::Color(185, 156, 235);
        this->suggestedMove = {-1, -1};
    }

    void handleEvent(const std::optional<sf::Event>&, sf::RenderWindow& window) override;
    void drawTexts(sf::RenderWindow& window) override;

    const sf::Color *handleStonePlacement(const std::optional<sf::Event> &event, sf::RenderWindow &window) override;

private:
    int moveNumber = 0;
};

#endif

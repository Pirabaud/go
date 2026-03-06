#ifndef DOUBLE_STONES_SCENE_HPP
#define DOUBLE_STONES_SCENE_HPP
#include "BoardScene.hpp"

class DoubleStonesScene : public BoardScene {
public:
    explicit DoubleStonesScene(sf::RenderWindow& window)
        : BoardScene(window) {
        backgroundColor = sf::Color(135, 206, 235);
        this->suggestedMove = {Board::SIZE / 2, Board::SIZE / 2};
    }
    void handleEvent(const std::optional<sf::Event>&, sf::RenderWindow& window) override;
    void drawTexts(sf::RenderWindow& window) override;
    bool handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

private:
    const sf::Color* winningColor = nullptr;
    int moveNumber = 0;
};

#endif
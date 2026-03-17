#ifndef PVP_SCENE_HPP
#define PVP_SCENE_HPP
#include "BoardScene.hpp"
#include "Button.hpp"
#include "DisplayService.hpp"


class PvPScene : public BoardScene {
public:
    explicit PvPScene(sf::RenderWindow& window)
        : BoardScene(window) {
        backgroundColor = sf::Color(206, 163, 70);
        this->suggestedMove = {Board::SIZE / 2, Board::SIZE / 2};
    }

    void handleEvent(const std::optional<sf::Event>&, sf::RenderWindow& window) override;
    void drawTexts(sf::RenderWindow& window) override;
    bool handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

protected:
    sf::Clock aiSuggestionTimer;
    bool needsAiSuggestion = false;
};


#endif //PVP_SCENE_HPP

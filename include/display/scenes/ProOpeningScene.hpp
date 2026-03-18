#ifndef GOMOKU_PROOPENINGSCENE_HPP
#define GOMOKU_PROOPENINGSCENE_HPP
#include "BoardScene.hpp"
#include "Button.hpp"
#include "DisplayService.hpp"


class ProOpeningScene : public BoardScene {
public:
    explicit ProOpeningScene(sf::RenderWindow& window)
        : BoardScene(window) {
        backgroundColor = sf::Color(206, 163, 70);
        this->suggestedMove = {-1, -1};
    }

    void handleEvent(const std::optional<sf::Event>&, sf::RenderWindow& window) override;
    void drawTexts(sf::RenderWindow& window) override;
    static bool isOutsideProZone(int row, int col);

    const sf::Color *handleStonePlacement(const std::optional<sf::Event> &event, sf::RenderWindow &window) override;

protected:
    sf::Clock aiSuggestionTimer;
    bool needsAiSuggestion = false;
    int moveNumber = 0;
};


#endif //GOMOKU_PROOPENINGSCENE_HPP

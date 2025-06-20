#ifndef BOARD_SCENE_HPP
#define BOARD_SCENE_HPP
#include "AbstractScene.hpp"
#include "Board.h"


class BoardScene final : public AbstractScene
{
public:
    BoardScene(sf::RenderWindow& window);

    void draw(sf::RenderWindow& window) override;
    void handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
    void update() override;

    static float PADDING;

private:
    Board board;
    sf::Color colorToPlay = sf::Color::Black;

    void drawBoard(sf::RenderWindow& window);
    void drawStones(sf::RenderWindow& window);
    void drawSingleColorStone(const std::array<uint32_t, 19>& stonesMask, sf::RenderWindow& window,
                              const sf::Color& color);
    void playMove(int row, int col);
    void nextTurn();
};


#endif //BOARD_SCENE_HPP

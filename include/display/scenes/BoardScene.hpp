#ifndef BOARD_SCENE_HPP
#define BOARD_SCENE_HPP
#include "AbstractScene.hpp"
#include "Board.h"
#include "structs/IllegalMoves.hpp"


class BoardScene : public AbstractScene {
public:
    void draw(sf::RenderWindow& window) override;

    static float PADDING;
    static float BOARD_SIZE;
    static float BOARD_SIZE_WITH_PADDING;
    static float CELL_SIZE;
    static float STONE_RADIUS;

protected:
    IllegalMoves::Type illegalMove = IllegalMoves::NONE;
    Board board;
    sf::Color colorToPlay = sf::Color::Black;
    BoardScene(sf::RenderWindow& window);

    virtual void drawTexts(sf::RenderWindow& window) = 0;
    virtual bool handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window) = 0;

    void drawBoard(sf::RenderWindow& window);
    void drawStones(sf::RenderWindow& window);
    void drawSingleColorStone(const std::array<uint32_t, 19>& stonesMask, sf::RenderWindow& window,
                              const sf::Color& color);

    [[nodiscard]] std::pair<int, int> getCellFromMousePosition(const sf::Vector2i& mousePos) const;
    void playMove(const int& row, const int& col);
    void nextTurn();
};


#endif //BOARD_SCENE_HPP

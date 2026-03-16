#ifndef BOARD_SCENE_HPP
#define BOARD_SCENE_HPP
#include <SFML/Audio/Sound.hpp>

#include "AbstractScene.hpp"
#include "JsonService.hpp"
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
    bool threeDetected = false;
    BoardScene(sf::RenderWindow& window);
    static void _loadSound(const std::string& filePath, sf::SoundBuffer*& buffer, sf::Sound*& sound);
    ~BoardScene() override;
    long lastAITimeMs = 0;
    Position suggestedMove = {-1, -1};
    sf::Sound *placeStoneSound = nullptr;
    sf::SoundBuffer *placeStoneSoundBuffer = nullptr;
    sf::Sound *captureSound = nullptr;
    sf::SoundBuffer *captureSoundBuffer = nullptr;
    sf::Sound *illegalMoveSound = nullptr;
    sf::SoundBuffer *illegalMoveSoundBuffer = nullptr;
    sf::Sound *winSound = nullptr;
    sf::SoundBuffer *winSoundBuffer = nullptr;
    sf::Sound *loseSound = nullptr;
    sf::SoundBuffer *loseSoundBuffer = nullptr;


    virtual void drawTexts(sf::RenderWindow& window) = 0;
    virtual bool handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window) = 0;

    void drawBoard(sf::RenderWindow& window);
    void drawStones(sf::RenderWindow& window);
    void drawSuggestedMove(sf::RenderWindow &window) const;

    void drawSingleColorStone(const std::array<uint64_t, 6>& bitBoard, sf::RenderWindow& window,
                              const sf::Color& color);

    [[nodiscard]] std::pair<int, int> getCellFromMousePosition(const sf::Vector2i& mousePos) const;

    Position handleAITurn();

    void playMove(Position pos);
    void nextTurn();
    IllegalMoves::Type getLegalMove(int row, int col);
};


#endif //BOARD_SCENE_HPP

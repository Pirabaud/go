#ifndef BOARD_SCENE_HPP
#define BOARD_SCENE_HPP
#include <SFML/Audio/Sound.hpp>

#include "AbstractScene.hpp"
#include "JsonService.hpp"
#include "Board.h"
#include "structs/IllegalMoves.hpp"

struct MoveInfo {
    Position pos;
    bool isBlack;
    std::vector<int> capturedIndices;
};


class BoardScene : public AbstractScene {
public:
    void draw(sf::RenderWindow& window) override;

    static void setBoardVars();

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
    ~BoardScene() override;
    long lastAITimeMs = 0;
    const sf::Color* winningColor = nullptr;
    Position suggestedMove = {-1, -1};
    std::stack<MoveInfo> pastMoves = {};
    std::stack<MoveInfo> futureMoves = {};
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
    sf::Sound *historySound = nullptr;
    sf::SoundBuffer *historySoundBuffer = nullptr;


    virtual void drawTexts(sf::RenderWindow& window) = 0;
    virtual bool handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window) = 0;

    bool handleHistoryEvent(const std::optional<sf::Event>&, sf::RenderWindow& window);
    void drawDefaultTexts(sf::RenderWindow& window) const;
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

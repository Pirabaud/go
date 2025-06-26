#ifndef SCENE_HPP
#define SCENE_HPP
#include "SFML/Graphics/RenderWindow.hpp"

class AbstractScene
{
protected:
    sf::Color backgroundColor = sf::Color::White;

public:
    virtual void handleEvent(const std::optional<sf::Event>&, sf::RenderWindow& window) = 0;
    // Called at each frame to draw the scene
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual ~AbstractScene() = default;
};


#endif //SCENE_HPP

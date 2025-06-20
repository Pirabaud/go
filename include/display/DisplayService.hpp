#ifndef DISPLAY_SERVICE_HPP
#define DISPLAY_SERVICE_HPP

#include "AbstractScene.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Window.hpp"

class DisplayService
{
public:
    DisplayService();
    ~DisplayService();

    void start();

    static void changeScene(AbstractScene* scene);

    static unsigned WINDOW_WIDTH;
    static unsigned WINDOW_HEIGHT;
    static AbstractScene* selectedScene;

private:
    void handleEvent(const std::optional<sf::Event>& event);
    void handleKeyboardEvents(sf::RenderWindow& window) const;

    bool isInitialized = false;
    sf::RenderWindow window;
};


#endif //DISPLAY_SERVICE_HPP

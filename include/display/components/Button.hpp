#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <functional>

#include "SFML/Audio.hpp"

#define BUTTON_COLOR {59, 166, 255}
#define BUTTON_HOVER_COLOR {79, 186, 255}

class Button
{
public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& label,
           const std::function<void()>& callback);

    void draw(sf::RenderWindow& window) const;
    void handleEvent(const std::optional<sf::Event>& event, const sf::RenderWindow& window);

private:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> onClick;

    sf::SoundBuffer* clickSoundBuffer;
    sf::Sound* clickSound;
};


#endif //BUTTON_HPP

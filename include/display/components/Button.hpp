#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <functional>

class Button
{
public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& label,
           const std::function<void()>& callback);

    void draw(sf::RenderWindow& window) const;
    void handleEvent(const std::optional<sf::Event>& event, const sf::RenderWindow& window) const;

private:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> onClick;
};


#endif //BUTTON_HPP

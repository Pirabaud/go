
#include "Button.hpp"

#include <iostream>

#include "utils/getSharedFont.hpp"

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& label,
               const std::function<void()>& callback): text(getSharedFont(), label)
{
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Blue);


    text.setString(label);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    const sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
    text.setPosition(position + size / 2.0f);

    onClick = callback;
}

void Button::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
    window.draw(text);
}

void Button::handleEvent(const std::optional<sf::Event>& event, const sf::RenderWindow& window) const
{
    if (!event.has_value()) return;
    const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
    if (!mouseButtonPressed) return;
    if (mouseButtonPressed->button == sf::Mouse::Button::Left)
    {
        const sf::Vector2f mousePos = window.mapPixelToCoords({
            mouseButtonPressed->position.x,
            mouseButtonPressed->position.y
        });
        if (shape.getGlobalBounds().contains(mousePos))
        {
            onClick();
        }
    }
}


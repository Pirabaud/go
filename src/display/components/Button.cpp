#include "Button.hpp"
#include "loadSound.hpp"
#include "utils/getSharedFont.hpp"

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& label,
               const std::function<void()>& callback) : text(getSharedFont(), label) {
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(BUTTON_COLOR);


    text.setString(label);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    const sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
    text.setPosition(position + size / 2.0f);

    onClick = callback;
    loadSound("../assets/click.mp3", clickSoundBuffer, clickSound);
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(text);
}

void Button::handleEvent(const std::optional<sf::Event>& event, const sf::RenderWindow& window) {
    if (!event.has_value()) return;
    const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>();
    if (mouseMoved) {
        const sf::Vector2f mousePos = window.mapPixelToCoords({mouseMoved->position.x, mouseMoved->position.y});
        if (shape.getGlobalBounds().contains(mousePos)) {
            shape.setFillColor(BUTTON_HOVER_COLOR);
        }
        else {
            shape.setFillColor(BUTTON_COLOR);
        }
        return;
    }

    const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
    if (!mouseButtonPressed) return;
    if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
        const sf::Vector2f mousePos = window.mapPixelToCoords({
            mouseButtonPressed->position.x,
            mouseButtonPressed->position.y
        });
        if (shape.getGlobalBounds().contains(mousePos)) {
            if (clickSound) {
                clickSound->play();
            }
            onClick();
        }
    }
}


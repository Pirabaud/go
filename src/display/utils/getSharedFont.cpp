
#include "SFML/Graphics/Font.hpp"

const sf::Font& getSharedFont()
{
    static sf::Font font;
    static bool loaded = false;
    if (!loaded)
    {
        if (!font.openFromFile("../assets/poppins_regular.ttf"))
        {
            throw std::runtime_error("Failed to load font");
        }
        loaded = true;
    }
    return font;
}

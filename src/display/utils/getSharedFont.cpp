#include <iostream>

#include "SFML/Graphics/Font.hpp"

const sf::Font& getSharedFont() {
    static sf::Font font;
    static bool loaded = false;

    if (loaded) return font;

    std::vector<std::string> fallbacks = {
        "../assets/poppins_regular.ttf",
        "assets/poppins_regular.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    };

    for (const auto& path : fallbacks) {
        if (font.openFromFile(path)) {
            loaded = true;
            return font;
        }
    }
    loaded = true;
    std::cerr << "ERROR [getSharedFont] : Impossible to load font. Text will not be visible." << std::endl;
    return font;
}

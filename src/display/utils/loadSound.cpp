#include <iostream>
#include "loadSound.hpp"

bool loadSound(const std::string& filePath, sf::SoundBuffer*& buffer, sf::Sound*& sound) {
    buffer = new sf::SoundBuffer();
    if (!buffer->loadFromFile(filePath)) {
        std::cerr << "Failed to load sound effect from " << filePath << "!" << std::endl;
        delete buffer;
        buffer = nullptr;
        sound = nullptr;
        return false;
    }
    sound = new sf::Sound(*buffer);
    return true;;
}

#ifndef GOMOKU_LOAD_SOUND_H
#define GOMOKU_LOAD_SOUND_H
#include "SFML/Audio.hpp"

bool loadSound(const std::string& filePath, sf::SoundBuffer*& buffer, sf::Sound*& sound);

#endif //GOMOKU_LOAD_SOUND_H

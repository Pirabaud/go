#ifndef GOMOKU_LOADSOUND_H
#define GOMOKU_LOADSOUND_H
#include "SFML/Audio.hpp"

bool loadSound(const std::string& filePath, sf::SoundBuffer*& buffer, sf::Sound*& sound);

#endif //GOMOKU_LOADSOUND_H
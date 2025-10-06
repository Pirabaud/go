//
// Created by Pierre Rabaud on 06/10/2025.
//

#include "../../include/services/AIService.hpp"

#include <iostream>
#include <random>

#include "CheckLegalMove.hpp"

int randomBetween(const int min, const int max) {
    // Générateur aléatoire basé sur un vrai random device
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister
    std::uniform_int_distribution<> dist(min, max); // distribution uniforme
    return dist(gen);
}

AiPlay AIService::AIPlay(Board board) {
    const clock_t start = clock();
    Position pos = {0, 0};

    for (int i = 0; i < 1000000; i++) {
        std::cout << "chargement...." << std::endl;
    }

    for (; CheckLegalMove::alreadyStone(pos, board.getGridBlack(), board.getGridWhite());) {
        pos.x = randomBetween(0, 18);
        pos.y = randomBetween(0, 18);
    }

    const clock_t end = clock();
    const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    return {pos, time};
}

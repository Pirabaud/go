//
// Created by pirabaud on 1/13/26.
//

#ifndef GOMOKU_SCORES_HPP
#define GOMOKU_SCORES_HPP

// Scores.hpp
namespace Scores {
    // Échelle exponentielle pour prioriser la victoire absolue
    static const int WIN        = 1000000;

    // Niveau "Game Over imminent"
    static const int OPEN_FOUR  = 100000;

    // Niveau "Menace critique" (Force une réponse)
    static const int FOUR       = 10000;

    // Niveau "Attaque forte"
    static const int OPEN_THREE = 5000;

    // Niveau "Construction"
    static const int BROKEN_THREE = 2000; // 3 avec un trou
    static const int THREE        = 1000;

    static const int OPEN_TWO     = 500;
    static const int TWO          = 100;
}
#endif //GOMOKU_SCORES_HPP
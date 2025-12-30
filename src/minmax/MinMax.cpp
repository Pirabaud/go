#include "MinMax.hpp"

#include <fstream>
#include <iostream>
#include <limits.h>
#include "JsonService.hpp"
#include "CheckWinService.hpp"
#include "HeuristicService.h"

int MinMax::MAX_DEPTH = 1;

MinMax::MinMax(Board &board) : board(board) {
}

MinMax::~MinMax() {
}

Board & MinMax::getBoard() const {
    return board;
}

std::pair<Position, long> MinMax::run(Position playerMove, json& decisionTree, std::vector<Position>& moveHistory) const {
    auto startTime = std::chrono::high_resolution_clock::now();

    int bestValue = INT_MIN;
    Position bestMove{-1, -1};
    const int rootHeuristic = HeuristicService::getHeuristicValue(board);

    auto possibleMoves = generatePossibleMoves(board);
    // #ifdef JSON_DEBUG
    // json children = json::array();
    // #endif

    for (const auto& move : possibleMoves) {

        board.addStoneWhite(move);

        json childTree = json::array();
        int moveValue = minimax(board, 1, INT_MIN, INT_MAX, true, childTree, move);
        std::cout << "return :" << move.x << " , " << move.y << " : " <<  moveValue << std::endl;
        //TODO remove stone

        // #ifdef JSON_DEBUG
        // JsonService::pushNode(children, moveValue, 0, INT_MIN, INT_MAX, move, childTree);
        // #endif

        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }

    std::cout << "BEST VALUE " << bestValue << std::endl;

    // Créer la racine avec le coup du joueur
    // #ifdef JSON_DEBUG
    // JsonService::pushNode(decisionTree, rootHeuristic, 0, INT_MIN, INT_MAX, playerMove, children);
    // json historyArray = json::array();
    // for (const auto& [x, y] : moveHistory) {
    //     historyArray.push_back({x, y});
    // }
    // decisionTree.push_back(historyArray);
    // #endif

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    long elapsedMs = duration.count();

    // #ifdef JSON_DEBUG
    // saveDecisionTree(decisionTree);
    // #endif
    return {bestMove, elapsedMs};
}


int MinMax::minimax(Board& currentBoard, int depth, int alpha, int beta, bool isMaximizing, json& tree, Position pos) {
    // Condition d'arrêt - NE PAS CRÉER DE NŒUD POUR LES FEUILLES
    auto possibleMoves = generatePossibleMoves(currentBoard);
    if (depth >= MAX_DEPTH) {
        //TODO check win and empty
        return HeuristicService::getHeuristicValue(currentBoard); // Juste retourner la valeur, pas de nœud créé
    }

    // #ifdef JSON_DEBUG
    // json children = json::array();
    // #endif
    int resultValue;

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (const auto& move : possibleMoves) {
            currentBoard.addStoneWhite(move);
            json childTree = json::array();
            int eval = minimax(currentBoard, depth + 1, alpha, beta, false, childTree, move);
            //TODO remove stone

            // #ifdef JSON_DEBUG
            // JsonService::pushNode(tree, eval, depth + 1, alpha, beta, move, childTree);
            // #endif
            // Si childTree est vide, c'est une feuille - on ne crée pas de nœud spécial

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if (beta <= alpha) break;
        }
        resultValue = maxEval;
    } else {
        int minEval = INT_MAX;
        for (const auto& move : possibleMoves) {
            currentBoard.addStoneBlack(move);
            json childTree = json::array();
            int eval = minimax(currentBoard, depth + 1, alpha, beta, true, childTree, move);
            //TODO remove stone

            // Créer un nœud enfant seulement si ce coup a des sous-arbres
            // #ifdef JSON_DEBUG
            // JsonService::pushNode(tree, eval, depth + 1, alpha, beta, move, childTree);
            // #endif
            // Si childTree est vide, c'est une feuille - on ne crée pas de nœud spécial

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            if (beta <= alpha) break;
        }
        resultValue = minEval;
    }

    // Créer le nœud courant seulement s'il a des enfants
    // Si pas d'enfants, tree reste vide = c'est une feuille

    return resultValue;
}

void MinMax::saveDecisionTree(const json& tree) {
    std::ofstream file("data.json");
    if (file.is_open()) {
        file << tree.dump(2);  // Pretty print avec indentation
        file.close();
        std::cout << "Arbre de décision sauvegardé dans data.json" << std::endl;
    } else {
        std::cerr << "Erreur: Impossible de sauvegarder l'arbre de décision" << std::endl;
    }
}
std::vector<Position> MinMax::generatePossibleMoves(Board& currentBoard) {
    std::vector<Position> moves;

    // Stratégie : seulement les cases autour des pierres existantes
    // pour réduire l'espace de recherche
    int searchRadius = 2; // Chercher dans un rayon de 2 cases autour des pierres

    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            //TODO check la stone existante
        }
    }
    // Si pas de pierres sur le board (début de partie), jouer au centre
    return moves;
}

bool MinMax::isNearExistingStone(Board& board, Position pos, int radius) {
    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
            int checkX = pos.x + dx;
            int checkY = pos.y + dy;

            if (checkX >= 0 && checkX < Board::SIZE && checkY >= 0 && checkY < Board::SIZE) {
                Position checkPos{checkX, checkY};
                //check autour
            }
        }
    }
    return false;
}
#include "MinMax.hpp"

#include <fstream>
#include <iostream>
#include <limits.h>
#include "JsonService.hpp"
#include "CheckWinService.hpp"
#include "HeuristicService.h"

int MinMax::MAX_DEPTH = 10;

MinMax::MinMax(Board &board) : board(board) {
}

MinMax::~MinMax() {
}

Board & MinMax::getBoard() const {
    return board;
}

Position MinMax::run(Position playerMove, json& decisionTree) const {
    int bestValue = INT_MIN;
    Position bestMove{-1, -1};
    const int rootHeuristic = HeuristicService::getHeuristicValue(board);

    auto possibleMoves = generatePossibleMoves(board);
    json children = json::array();

    for (const auto& move : possibleMoves) {
        Board newBoard = board;
        newBoard.addStoneWhite(move);

        json childTree = json::array();
        int moveValue = minimax(newBoard, 1, INT_MIN, INT_MAX, false, childTree);

        JsonService::pushNode(children, moveValue, 1, INT_MIN, INT_MAX, move, childTree);

        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }

    // Créer la racine avec le coup du joueur
    JsonService::pushNode(decisionTree, rootHeuristic, 0, INT_MIN, INT_MAX, playerMove, children);

    saveDecisionTree(decisionTree);
    std::cout << "IA joue en (" << bestMove.x << ", " << bestMove.y << ") avec score: " << bestValue << std::endl;
    return bestMove;
}


int MinMax::minimax(Board& currentBoard, int depth, int alpha, int beta, bool isMaximizing, json& tree) const {
    // Évaluation de la position actuelle
    int currentHeuristic = HeuristicService::getHeuristicValue(currentBoard);

    // Condition d'arrêt - NE PAS CRÉER DE NŒUD POUR LES FEUILLES
    if (depth >= MAX_DEPTH || abs(currentHeuristic) >= WIN_WEIGHT || generatePossibleMoves(currentBoard).empty()) {
        return currentHeuristic; // Juste retourner la valeur, pas de nœud créé
    }

    auto possibleMoves = generatePossibleMoves(currentBoard);
    json children = json::array();
    int resultValue;

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (const auto& move : possibleMoves) {
            Board newBoard = currentBoard;
            newBoard.addStoneWhite(move);

            json childTree = json::array();
            int eval = minimax(newBoard, depth + 1, alpha, beta, false, childTree);

            JsonService::pushNode(tree, eval, depth + 1, alpha, beta, move, childTree);
            // Si childTree est vide, c'est une feuille - on ne crée pas de nœud spécial

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if (beta <= alpha) break;
        }
        resultValue = maxEval;
    } else {
        int minEval = INT_MAX;
        for (const auto& move : possibleMoves) {
            Board newBoard = currentBoard;
            newBoard.addStoneBlack(move);

            json childTree = json::array();
            int eval = minimax(newBoard, depth + 1, alpha, beta, true, childTree);

            // Créer un nœud enfant seulement si ce coup a des sous-arbres
            JsonService::pushNode(tree, eval, depth + 1, alpha, beta, move, childTree);
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

void MinMax::saveDecisionTree(const json& tree) const {
    std::ofstream file("data.json");
    if (file.is_open()) {
        file << tree.dump(2);  // Pretty print avec indentation
        file.close();
        std::cout << "Arbre de décision sauvegardé dans data.json" << std::endl;
    } else {
        std::cerr << "Erreur: Impossible de sauvegarder l'arbre de décision" << std::endl;
    }
}

std::vector<Position> MinMax::generatePossibleMoves(Board& currentBoard) const {
    std::vector<Position> moves;

    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            Position pos{x, y};
            if (!currentBoard.isWhiteStoneAt(pos) && !currentBoard.isBlackStoneAt(pos)) {
                moves.push_back(pos);
            }
        }
    }

    return moves;
}
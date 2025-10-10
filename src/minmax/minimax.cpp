#include "GameState.hpp"
#include "HeuristicService.h"

int minimax(GameState* gameState) {
    //TODO check if gameState is null
    if (gameState->isTerminal()) {
        return HeuristicService::getHeuristicValue(gameState->getBoard());
    }

    if (!gameState->isMaximizing()) {
        // Min
        int minEval = 1000000000;
        for (auto move : gameState->getPossibleMoves()) {
            GameState* newState = gameState->play(move);
            int eval = minimax(newState);
            minEval = std::min(minEval, eval);

        }
        return minEval;
    } else {
        // Max
        int maxEval = -1000000000;
        for (auto move : gameState->getPossibleMoves()) {
            GameState* newState = gameState->play(move);
            int eval = minimax(newState);
            maxEval = std::max(maxEval, eval);

        }
        return maxEval;
    }
}

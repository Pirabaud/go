
#include "../../include/minmax/GameState.hpp"

#include <iostream>
#include <ostream>

#include "CheckMoveService.hpp"
#include "CheckWinService.hpp"
#include "minimax.hpp"
#include "MinMax.hpp"

GameState::GameState(GameState* _parent, Board& board, Position moveToMake, bool newMaximizing) : board(board) {
    parent = _parent;
    if (parent) {
        parent->children.push_back(this);
        depth = parent->depth + 1;
        maximizing = !parent->maximizing;
    } else {
        depth = 0;
        maximizing = newMaximizing;
    }
    move = moveToMake;
    if (!maximizing) {
        board.addStoneBlack(moveToMake);
    } else {
        board.addStoneWhite(moveToMake);
    }
}

Position GameState::getBestMove() {
    Position bestMove{};
    int bestScore = maximizing ? -1000000 : 1000000;

    for (auto move : getPossibleMoves()) {
        GameState *newState = play(move);
        int score = minimax(newState);

        if (!maximizing) {
            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
            }
        } else {
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
        delete newState;
    }
    std::cout << "(" << bestMove.x << "," << bestMove.y << ")" << std::endl;
    return bestMove;
}

GameState * GameState::play(Position pos) {
    GameState* newGameState = new GameState(this, board, pos);
    return newGameState;
}

void GameState::setDepth(int newDepth) {
    depth = newDepth;
}

Position GameState::getMove() const {
    return move;
}

Board & GameState::getBoard() {
    return board;
}

bool GameState::isBlack() const {
    return !maximizing;
}

bool GameState::isMaximizing() const {
    return maximizing;
}

bool GameState::isTerminal() const {
    if (depth == MinMax::MAX_DEPTH) return true;
    if (CheckWinService::isWin(board)) return true;
    if (getPossibleMoves().empty()) return true;
    return false;
}

std::vector<Position> GameState::getPossibleMoves() const {
    std::vector<Position> possibleMoves;

    // Get all positions in a square of distance 4 around position
    for (int x = std::max(0, move.x - 4); x < std::min(Board::SIZE, move.x + 4); ++x) {
        for (int y = std::max(0, move.y - 4); y < std::min(Board::SIZE, move.y + 4); ++y) {
            if (move.x == x && move.y == y) continue;
            if (CheckMoveService::isLegalMove({x, y}, board, !maximizing) != IllegalMoves::NONE) continue;
            possibleMoves.push_back({x, y});
        }
    }
    return possibleMoves;
}

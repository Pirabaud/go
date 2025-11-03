#include "MoveGenerator.hpp"
#include <algorithm>

std::vector<Position> MoveGenerator::generateMoves(const Board& board, int searchRadius) {
    std::vector<Position> moves;
    moves.reserve(20);

    bool hasStones = false;
    for (int x = 0; x < Board::SIZE && !hasStones; x++) {
        for (int y = 0; y < Board::SIZE && !hasStones; y++) {
            if (board.isWhiteStoneAt({x, y}) || board.isBlackStoneAt({x, y})) {
                hasStones = true;
            }
        }
    }

    if (!hasStones) {
        moves.push_back({Board::SIZE / 2, Board::SIZE / 2});
        return moves;
    }

    // Générer TOUS les moves candidats proches des pierres
    // Pas de limite ici - le move ordering fera le tri
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 0; y < Board::SIZE; y++) {
            Position pos{x, y};
            if (!board.isWhiteStoneAt(pos) && !board.isBlackStoneAt(pos)) {
                if (isNearStone(board, pos, searchRadius)) {
                    moves.push_back(pos);
                }
            }
        }
    }

    return moves;
}

bool MoveGenerator::isNearStone(const Board& board, Position pos, int radius) {
    int minX = std::max(0, pos.x - radius);
    int maxX = std::min(Board::SIZE - 1, pos.x + radius);
    int minY = std::max(0, pos.y - radius);
    int maxY = std::min(Board::SIZE - 1, pos.y + radius);

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            Position checkPos{x, y};
            if (board.isWhiteStoneAt(checkPos) || board.isBlackStoneAt(checkPos)) {
                return true;
            }
        }
    }
    return false;
}

#include "ZobristHash.hpp"

ZobristHash::ZobristHash() {
    std::mt19937_64 rng(0x1234567890ABCDEFULL);
    std::uniform_int_distribution<uint64_t> dist;

    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            whiteStones[x][y] = dist(rng);
            blackStones[x][y] = dist(rng);
        }
    }
}

ZobristHash& ZobristHash::getInstance() {
    static ZobristHash instance;
    return instance;
}

uint64_t ZobristHash::getHash(const Board& board) const {
    uint64_t hash = 0;

    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            Position pos{x, y};
            if (board.isWhiteStoneAt(pos)) {
                hash ^= whiteStones[x][y];
            } else if (board.isBlackStoneAt(pos)) {
                hash ^= blackStones[x][y];
            }
        }
    }

    return hash;
}

uint64_t ZobristHash::updateHash(uint64_t currentHash, Position pos, bool isWhite) const {
    if (isWhite) {
        return currentHash ^ whiteStones[pos.x][pos.y];
    } else {
        return currentHash ^ blackStones[pos.x][pos.y];
    }
}

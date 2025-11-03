#ifndef ZOBRIST_HASH_HPP
#define ZOBRIST_HASH_HPP

#include <cstdint>
#include <random>
#include "Board.h"
#include "Position.hpp"

class ZobristHash {
public:
    static ZobristHash& getInstance();

    uint64_t getHash(const Board& board) const;
    uint64_t updateHash(uint64_t currentHash, Position pos, bool isWhite) const;

private:
    ZobristHash();

    static constexpr int SIZE = Board::SIZE;
    uint64_t whiteStones[SIZE][SIZE];
    uint64_t blackStones[SIZE][SIZE];
};

#endif

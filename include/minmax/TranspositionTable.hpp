#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include <cstdint>
#include <vector>
#include "Position.hpp"

enum class NodeType : uint8_t {
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
};

struct TTEntry {
    uint64_t key;
    int score;
    int depth;
    NodeType type;
    Position bestMove;

    TTEntry() : key(0), score(0), depth(-1), type(NodeType::EXACT), bestMove{-1, -1} {}
};

class TranspositionTable {
public:
    explicit TranspositionTable(size_t sizeInMB = 128);

    void store(uint64_t hash, int score, int depth, NodeType type, Position bestMove);
    bool probe(uint64_t hash, int depth, int alpha, int beta, int& score, Position& bestMove);
    void clear();

private:
    std::vector<TTEntry> table;
    size_t size;

    size_t getIndex(uint64_t hash) const;
};

#endif

#include "TranspositionTable.hpp"

TranspositionTable::TranspositionTable(size_t sizeInMB) {
    size = (sizeInMB * 1024 * 1024) / sizeof(TTEntry);
    table.resize(size);
}

size_t TranspositionTable::getIndex(uint64_t hash) const {
    return hash % size;
}

void TranspositionTable::store(uint64_t hash, int score, int depth, NodeType type, Position bestMove) {
    size_t index = getIndex(hash);
    TTEntry& entry = table[index];

    if (entry.depth <= depth) {
        entry.key = hash;
        entry.score = score;
        entry.depth = depth;
        entry.type = type;
        entry.bestMove = bestMove;
    }
}

bool TranspositionTable::probe(uint64_t hash, int depth, int alpha, int beta, int& score, Position& bestMove) {
    size_t index = getIndex(hash);
    const TTEntry& entry = table[index];

    if (entry.key == hash && entry.depth >= depth) {
        bestMove = entry.bestMove;

        if (entry.type == NodeType::EXACT) {
            score = entry.score;
            return true;
        } else if (entry.type == NodeType::LOWER_BOUND && entry.score >= beta) {
            score = entry.score;
            return true;
        } else if (entry.type == NodeType::UPPER_BOUND && entry.score <= alpha) {
            score = entry.score;
            return true;
        }
    }

    if (entry.key == hash) {
        bestMove = entry.bestMove;
    }

    return false;
}

void TranspositionTable::clear() {
    table.clear();
    table.resize(size);
}

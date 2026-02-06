#include "TranspositionTable.hpp"

TranspositionTable::TranspositionTable() {
    table.reserve(SIZE);
}

void TranspositionTable::store(uint64_t zobristKey, int depth, int score, TTFlag flag, int bestMove) {
    const int index = zobristKey & (SIZE - 1); // Modulo SIZE
    table[index] = {zobristKey, depth, score, flag, bestMove};
}

const TTEntry* TranspositionTable::retrieve(uint64_t zobristKey) const {
    const int index = zobristKey & (SIZE - 1); // Modulo SIZE
    const TTEntry& entry = table[index];

    if (entry.zobristKey == zobristKey) {
        return &entry;
    }

    return nullptr;
}

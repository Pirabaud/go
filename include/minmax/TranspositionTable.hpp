#ifndef GOMOKU_TRANSPOSITIONTABLE_H
#define GOMOKU_TRANSPOSITIONTABLE_H


#include <vector>

enum TTFlag { EXACT, LOWERBOUND, UPPERBOUND };

struct TTEntry {
    uint64_t zobristKey;
    int depth;
    int score;
    TTFlag flag;
    int bestMove;
};

class TranspositionTable {
public:
    static const int SIZE = 1 << 23; // ~16 million entries
    std::vector<TTEntry> table;

    TranspositionTable();

    void store(uint64_t zobristKey, int depth, int score, TTFlag flag, int bestMove);
    const TTEntry* retrieve(uint64_t zobristKey) const;

};


#endif //GOMOKU_TRANSPOSITIONTABLE_H
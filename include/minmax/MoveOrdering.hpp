//
// Created by pirabaud on 2/17/26.
//

#ifndef GOMOKU_MOVEORDERING_HPP
#define GOMOKU_MOVEORDERING_HPP
#include <cstdint>
#include <vector>

struct MoveList {
    int moves[256];
    int score[256];

};

class MoveOrdering {
public:
    std::vector<int> generateMoveOrder(const std::array<uint64_t, 6> &allyBoard, const std::array<uint64_t, 6> &enemyBoard);
private:
    void getWin(const std::array<uint64_t, 6> &allyBoard, const std::array<uint64_t, 6> &enemyBoard);
    void getFreeFour(const std::array<uint64_t, 6> &allyBoard, const std::array<uint64_t, 6> &enemyBoard);
    std::array<uint64_t, 6> newMoves(const std::array<uint64_t, 6> &occupied);
};

#endif //GOMOKU_MOVEORDERING_HPP
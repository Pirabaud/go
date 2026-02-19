//
// Created by pirabaud on 6/19/25.
//

#include "Board.h"

#include <bitset>
#include <iomanip>
#include <iostream>
#include <random>

#include "HeuristicService.h"
#include "Position.hpp"

uint64_t Board::ZOBRIST_TABLE[361][2] = {};

int Board::getWhiteCaptured() const {
    return this->whiteStoneCaptured;
}

int Board::getBlackCaptured() const {
    return this->blackStoneCaptured;
}

std::array<uint64_t, 6> &Board::getBitBoardWhite() {
    return this->bitBoardWhite;
}

const std::array<uint64_t, 6> &Board::getBitBoardWhite() const {
    return this->bitBoardWhite;
}

std::array<uint64_t, 6> &Board::getBitBoardBlack() {
    return this->bitBoardBlack;
}

const std::array<uint64_t, 6> &Board::getBitBoardBlack() const {
    return this->bitBoardBlack;
}

void Board::addStoneBlack(const Position pos) {
    const int global_index = pos.x * (SIZE + 1) + pos.y;

    // 1. On regarde les deux couleurs AVANT
    const int blackBefore = HeuristicService::evaluatePosition(*this, global_index, true);
    const int whiteBefore = HeuristicService::evaluatePosition(*this, global_index, false);

    // 2. On pose la pierre
    this->bitBoardBlack[global_index / 64] |= 1ULL << (global_index % 64);
    this->updateCurrentZobristKey(global_index, true);

    // 3. On regarde les deux couleurs APRES
    const int blackAfter = HeuristicService::evaluatePosition(*this, global_index, true);
    const int whiteAfter = HeuristicService::evaluatePosition(*this, global_index, false);

    // 4. LE VRAI DELTA ABSOLU
    // Ce que Noir a gagné MOINS ce que Blanc a gagné
    updateScore((blackAfter - blackBefore) - (whiteAfter - whiteBefore));
}

void Board::addStoneWhite(const Position pos) {
    const int global_index = pos.x * (SIZE + 1) + pos.y;

    const int blackBefore = HeuristicService::evaluatePosition(*this, global_index, true);
    const int whiteBefore = HeuristicService::evaluatePosition(*this, global_index, false);

    this->bitBoardWhite[global_index / 64] |= 1ULL << (global_index % 64);
    this->updateCurrentZobristKey(global_index, false);

    const int blackAfter = HeuristicService::evaluatePosition(*this, global_index, true);
    const int whiteAfter = HeuristicService::evaluatePosition(*this, global_index, false);

    // MAGIE : C'EST EXACTEMENT LA MEME FORMULE !
    // Si Blanc a joué un bon coup, whiteAfter sera plus grand que whiteBefore.
    // Donc on va soustraire un nombre positif, le globalScore va baisser (ce qui est le but de Blanc !)
    updateScore((blackAfter - blackBefore) - (whiteAfter - whiteBefore));
}

void Board::addStoneWhite(const int index) {
    this->bitBoardWhite[index / 64] |= 1ULL << (index % 64);
    this->updateCurrentZobristKey(index, false);
}

void Board::addStoneBlack(const int index) {
    this->bitBoardBlack[index / 64] |= 1ULL << (index % 64);
    this->updateCurrentZobristKey(index, true);
}

void Board::removeWhiteStone(const int index) {
    this->bitBoardWhite[index / 64] &= ~(1ULL << (index % 64));
    this->updateCurrentZobristKey(index, false);
}

void Board::removeBlackStone(const int index) {
    this->bitBoardBlack[index / 64] &= ~(1ULL << (index % 64));
    this->updateCurrentZobristKey(index, true);
}

template<int Offset>
void updatePattern(int& index, int center, int dir, const std::array<uint64_t, 6>& ally, const std::array<uint64_t, 6>& enemy) {

    // 1. Calcul des coordonnées de départ (x0, y0)
    int x0 = center % 20; // Board::SIZE
    int y0 = center / 20;

    // 2. Calcul du déplacement (dx, dy) selon la direction 'dir'
    int dx = 0, dy = 0;
    if (dir == 1)       { dx = 1;  dy = 0; } // Horizontal
    else if (dir == 19) { dx = -1; dy = 1; } // Diag / (Attention à ton sens, vérifie tes constantes)
    else if (dir == 20) { dx = 0;  dy = 1; } // Vertical
    else if (dir == 21) { dx = 1;  dy = 1; } // Diag \

    // 3. Calcul de la position cible (x, y)
    int x = x0 + (Offset * dx);
    int y = y0 + (Offset * dy);

    // 4. VÉRIFICATION DES BORNES 2D (C'est ça qui manquait !)
    if (x < 0 || x >= 19 || y < 0 || y >= 19) {
        // C'est un vrai MUR
        index = (index << 2) | 3; // 3 = WALL (HeuristicService::WALL_BITS_MASK)
        return;
    }

    // 5. Lecture du bitboard (maintenant c'est sûr)
    const int pos = y * 19 + x; // Reconversion en 1D propre
    const int word = pos >> 6;
    const int bit = pos & 0x3F;

    const int a = (ally[word] >> bit) & 1;
    const int e = (enemy[word] >> bit) & 1;

    index = (index << 2) | (e << 1 | a);
}

int Board::getPatternIndex(const int positionIndex, const bool isBlackPlayer, const int direction) const {


    // direction : 1 (horizontal), 20 (vertical), 21 (Diag /), 19 (Diag \)
    int index = 0;
    // Use ALLY_BITS_MASK and ENEMY_BITS_MASK to build the index
    // Bitboards are of type std::array<uint64_t, 6>
    const std::array<uint64_t, 6> &allyBitBoard = isBlackPlayer ? this->bitBoardBlack : this->bitBoardWhite;
    const std::array<uint64_t, 6> &enemyBitBoard = isBlackPlayer ? this->bitBoardWhite : this->bitBoardBlack;
    updatePattern<-5>(index, positionIndex, direction, allyBitBoard, enemyBitBoard);
    updatePattern<-4>(index, positionIndex, direction, allyBitBoard, enemyBitBoard);
    updatePattern<-3>(index, positionIndex, direction, allyBitBoard, enemyBitBoard);
    updatePattern<-2>(index, positionIndex, direction, allyBitBoard, enemyBitBoard);
    updatePattern<-1>(index, positionIndex, direction, allyBitBoard, enemyBitBoard);
    updatePattern<0>(index, positionIndex, direction, allyBitBoard, enemyBitBoard);
    updatePattern<1>(index, positionIndex, direction, allyBitBoard, enemyBitBoard);
    updatePattern<2>(index, positionIndex, direction, allyBitBoard, enemyBitBoard);
    updatePattern<3>(index, positionIndex, direction, allyBitBoard, enemyBitBoard);
    updatePattern<4>(index, positionIndex, direction, allyBitBoard, enemyBitBoard);
    updatePattern<5>(index, positionIndex, direction, allyBitBoard, enemyBitBoard);

    // Print index in binary
    return index;
}


void Board::addCaptures(const bool isStoneWhite, const int stoneCount) {
    if (isStoneWhite) {
        this->whiteStoneCaptured += stoneCount;
    } else {
        this->blackStoneCaptured += stoneCount;
    }
}

void Board::removeCaptures(const bool isStoneWhite, const int stoneCount) {
    if (!isStoneWhite) {
        this->whiteStoneCaptured -= stoneCount;
    } else {
        this->blackStoneCaptured -= stoneCount;
    }
}

bool Board::isEmpty() const {
    for (int i = 0; i < 6; i++) {
        if (bitBoardWhite[i] != 0 || bitBoardBlack[i] != 0) {
            return false;
        }
    }
    return true;
}

std::array<uint64_t, 6> Board::shift_right_board(const std::array<uint64_t, 6> &currentBitboard, const int shift) {
    std::array<uint64_t, 6> board{};

    uint64_t carry = 0;
    for (int i = 5; i >= 0; i--) {
        const uint64_t nextCarry = currentBitboard[i] << (64 - shift);
        board[i] = currentBitboard[i] >> shift | carry;
        carry = nextCarry;
    }

    return board;
}

std::array<uint64_t, 6> Board::shift_left_board(const std::array<uint64_t, 6> &currentBitboard, const int shift) {
    std::array<uint64_t, 6> board{};

    uint64_t carry = 0;
    for (int i = 0; i < 6; i++) {
        const uint64_t nextCarry = currentBitboard[i] >> (64 - shift);
        board[i] = (currentBitboard[i] << shift) | carry;
        carry = nextCarry;
    }

    return board;
}

std::array<uint64_t, 6> Board::bitBoardAnd(
    const std::array<uint64_t, 6> &bitBoard1,
    const std::array<uint64_t, 6> &bitBoard2) {
    std::array<uint64_t, 6> result{};
    for (int i = 0; i < 6; i++) {
        result[i] = bitBoard1[i] & bitBoard2[i];
    }
    return result;
}

std::array<uint64_t, 6> Board::bitBoardOr(
    const std::array<uint64_t, 6> &bitBoard1,
    const std::array<uint64_t, 6> &bitBoard2) {
    std::array<uint64_t, 6> result{};
    for (int i = 0; i < 6; i++) {
        result[i] = bitBoard1[i] | bitBoard2[i];
    }
    return result;

}

int Board::getGlobalIndex(const Position pos) {
    return pos.x * (SIZE + 1) + pos.y;
}

bool Board::isBitAt(const std::array<uint64_t, 6> &bitBoard, const int globalIndex) {
    if (globalIndex < 0 || globalIndex >= 384) return false;
    const int arrayIndex = globalIndex / 64;
    const int bitIndex = globalIndex % 64;
    return (bitBoard[arrayIndex] & (1ULL << bitIndex)) != 0;
}

void Board::clearBitAt(std::array<uint64_t, 6> &bitBoard, const int globalIndex) {
    const int arrayIndex = globalIndex / 64;
    const int bitIndex = globalIndex % 64;
    bitBoard[arrayIndex] &= ~(1ULL << bitIndex);
}

void Board::initZobrist() {
    std::mt19937_64 rng(42); // Fixed seed for reproducibility
    std::uniform_int_distribution<uint64_t> dist;
    for (auto & indecesPosition : ZOBRIST_TABLE) {
        indecesPosition[0] = dist(rng); // Black stone
        indecesPosition[1] = dist(rng); // White stone
    }
}

void Board::updateCurrentZobristKey(int index, bool isBlack) {
    currentZobristKey ^= ZOBRIST_TABLE[index][isBlack ? 1 : 0];
}

void Board::updateScore(const int score) {
    globalScore += score;
}

int Board::getScore() const {
    return globalScore;
}

std::ostream &operator<<(std::ostream &os, const Board &board) {
    for (int i = 0; i < Board::SIZE; i++) {
        os << std::setw(2) << std::setfill(' ') << i;
        os << ':';
        for (int j = 0; j < Board::SIZE; j++) {
            if (Board::isBitAt(board.getBitBoardWhite(), Board::getGlobalIndex({i, j}))) {
                os << 'W';
            }
            else if (Board::isBitAt(board.getBitBoardBlack(), Board::getGlobalIndex({i, j}))) {
                os << 'B';
            }
            else {
                os << 'O';
            }
        }
        os << '\n';
    }
    return os;
}

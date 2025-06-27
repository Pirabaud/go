#include "../../include/services/CheckMoveService.hpp"

#include <bitset>
#include <iostream>


IllegalMoves::Type CheckMoveService::isLegalMove(const int& x, const int& y,
                                                 const std::array<uint32_t, Board::SIZE>& gridBlack,
                                                 const std::array<uint32_t, Board::SIZE>& gridWhite,
                                                 const bool& isBlack) {
    if (notInBoard(x, y)) {
        return IllegalMoves::Type::NOT_IN_BOARD;
    }
    if (alreadyStone(x, y, gridBlack, gridWhite)) {
        return IllegalMoves::Type::OCCUPIED;
    }
    if (checkDirectionCreatingCapture(
        x,
        y,
        isBlack ? gridBlack : gridWhite,
        isBlack ? gridWhite : gridBlack)) {
        return IllegalMoves::Type::CREATE_CAPTURE;
    }
    return IllegalMoves::Type::NONE;
}

bool CheckMoveService::notInBoard(const int& x, const int& y) {
    if (x < 0 || x >= Board::SIZE || y < 0 || y >= Board::SIZE) {
        return true;
    }
    return false;
}

bool CheckMoveService::alreadyStone(const int& x, const int& y, const std::array<uint32_t, Board::SIZE>& gridBlack,
                                    const std::array<uint32_t, Board::SIZE>& gridWhite) {
    uint32_t checkStone = 1u << (Board::SIZE - 1 - y);
    if (checkStone & gridBlack.at(x) || checkStone & gridWhite.at(x)) {
        return true;
    }
    return false;
}

bool CheckMoveService::checkDirectionCreatingCapture(const int& x, const int& y,
                                                     const std::array<uint32_t, Board::SIZE>& gridColor,
                                                     const std::array<uint32_t, Board::SIZE>& gridOpposite) {
    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto& [dx, dy] : directions) {
        if (checkCapture(x, y, gridColor, gridOpposite, dx, dy)) {
            return true;
        }
        if (checkCapture(x, y, gridColor, gridOpposite, -dx, -dy)) {
            return true;
        }
    }
    return false;
}

inline bool checkBitSet(const uint32_t& val, const int& bit) {
    return bit >= 0 && bit < Board::SIZE && val & (1u << bit);
}

bool CheckMoveService::checkCapture(const int& x, const int& y, const std::array<uint32_t, Board::SIZE>& gridColor,
                                    const std::array<uint32_t, Board::SIZE>& gridOpposite, const int& dx,
                                    const int& dy) {
    if (x + dx < 0 ||
        x - dx < 0 ||
        x + dx * 2 < 0 ||
        x + dx * 2 >= Board::SIZE ||
        x - dx >= Board::SIZE)
        return false;

    const uint32_t lineColorAdjacent = gridColor.at(x + dx);
    const uint32_t lineOppositeAdjacent1 = gridOpposite.at(x + dx * 2);
    const uint32_t lineOppositeAdjacent2 = gridOpposite.at(x - dx);
    const int bitPos = Board::SIZE - y - 1;

    const int maskColorAdjacent = bitPos - dy;
    const int maskOppositeAdjacent1 = bitPos - dy * 2;
    const int maskOppositeAdjacent2 = bitPos - dy * -1;

    if (checkBitSet(lineColorAdjacent, maskColorAdjacent)
        && checkBitSet(lineOppositeAdjacent1, maskOppositeAdjacent1)
        && checkBitSet(lineOppositeAdjacent2, maskOppositeAdjacent2)) {
        return true;
    }
    return false;
}


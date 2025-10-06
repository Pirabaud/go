#include "../../include/services/CheckLegalMove.hpp"

#include <bitset>
#include <iostream>


#include "CaptureService.hpp"


IllegalMoves::Type CheckLegalMove::isLegalMove(Position pos,
                                                 Board &board,
                                                 const bool &isBlack) {
    if (notInBoard(pos)) {
        return IllegalMoves::Type::NOT_IN_BOARD;
    }
    if (alreadyStone(pos, board.getGridBlack(), board.getGridWhite())) {
        return IllegalMoves::Type::OCCUPIED;
    }
    if (CaptureService::resolveCaptureAtPosition(board, pos, isBlack)) {
        return IllegalMoves::Type::CREATE_CAPTURE;
    }

     if (checkDoubleThree(pos,
         isBlack ? board.getGridBlack() : board.getGridWhite(),
         isBlack ? board.getGridWhite() : board.getGridBlack())) {
         return IllegalMoves::Type::DOUBLE_FREE_CAPTURE;
     }

    return IllegalMoves::Type::NONE;
}

bool CheckLegalMove::notInBoard(Position pos) {
    if (pos.x < 0 || pos.x >= Board::SIZE || pos.y < 0 || pos.y >= Board::SIZE) {
        return true;
    }
    return false;
}

bool CheckLegalMove::alreadyStone(Position pos, const Board::StoneMask& gridBlack,
                                   Board::StoneMask& gridWhite) {
    uint32_t checkStone = 1u << (Board::SIZE - 1 - pos.y);
    if (checkStone & gridBlack.at(pos.x) || checkStone & gridWhite.at(pos.x)) {
        return true;
    }
    return false;
}

bool CheckLegalMove::checkDirectionCreatingCapture(Position pos,
                                                     const std::array<uint32_t, Board::SIZE>& gridColor,
                                                     const std::array<uint32_t, Board::SIZE>& gridOpposite) {
    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto& [dx, dy] : directions) {
        if (checkCapture(pos, gridColor, gridOpposite, Position{dx, dy})) {
            return true;
        }
        if (checkCapture(pos, gridColor, gridOpposite, Position{-dx, -dy})) {
            return true;
        }
    }
    return false;
}

inline bool checkBitSet(const uint32_t& val, const int& bit) {
    return bit >= 0 && bit < Board::SIZE && val & (1u << bit);
}

bool CheckLegalMove::checkCapture(Position pos, const std::array<uint32_t, Board::SIZE>& gridColor,
                                    const std::array<uint32_t, Board::SIZE>& gridOpposite, Position direction) {
    if (pos.x + direction.x < 0 ||
        pos.x - direction.x < 0 ||
        pos.x + direction.x * 2 < 0 ||
        pos.x + direction.x * 2 >= Board::SIZE ||
        pos.x - direction.x >= Board::SIZE)
        return false;

    const uint32_t lineColorAdjacent = gridColor.at(pos.x + direction.x);
    const uint32_t lineOppositeAdjacent1 = gridOpposite.at(pos.x + direction.x * 2);
    const uint32_t lineOppositeAdjacent2 = gridOpposite.at(pos.x - direction.x);
    const int bitPos = Board::SIZE - pos.y - 1;

    const int maskColorAdjacent = bitPos - direction.y;
    const int maskOppositeAdjacent1 = bitPos - direction.y * 2;
    const int maskOppositeAdjacent2 = bitPos - direction.y * -1;

    if (checkBitSet(lineColorAdjacent, maskColorAdjacent)
        && checkBitSet(lineOppositeAdjacent1, maskOppositeAdjacent1)
        && checkBitSet(lineOppositeAdjacent2, maskOppositeAdjacent2)) {
        return true;
    }
    return false;
}

bool CheckLegalMove::checkDoubleThree(Position pos, Board::StoneMask grid, Board::StoneMask gridOpposite) {

    if (AlignmentChecker::detectAlignment(pos, 3, grid, gridOpposite) != Alignment::FREE) {
        return false;
    }

    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto& [dx, dy] : directions) {
        if (AlignmentChecker::detectAlignment({pos.x + dx, pos.y + dy}, 3, grid, gridOpposite) != Alignment::NOTALIGN) {
            return true;
        }

        if (AlignmentChecker::detectAlignment({pos.x - dx, pos.y - dy}, 3, grid, gridOpposite) != Alignment::NOTALIGN) {
            return true;
        }
    }

    return false;
}




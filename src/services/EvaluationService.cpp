//
// Created by Pierre Rabaud on 17/10/2025.
//

#include "../../include/services/EvaluationService.hpp"

#include <iostream>

#include "AlignmentChecker.hpp"

int EvaluationService::evaluate(const Board &board, const Position pos, const bool color) {
    static int count = 0;
    count++;
    //std::cout << count << std::endl;
    int result = 0;

    result += checkAlignmentEvaluation(board, pos, color);
    result += checlAllAlignmentOpp(board, pos, color);

    return result;
}

int EvaluationService::checkAlignmentEvaluation(Board board, const Position pos, const bool color) {
    Board::StoneMask grid = color == 1 ? board.getGridWhite() : board.getGridBlack();
    Board::StoneMask gridOpposite = color == 1 ? board.getGridBlack() : board.getGridWhite();
    int result = 0;
    int blocked = 2;
    AlignmentChecker::Result countAlignment1;
    AlignmentChecker::Result countAlignment2;
    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };
    for (auto [dx, dy] : directions) {
        countAlignment1 = AlignmentChecker::countDirection(pos, {dx, dy}, 5, grid, gridOpposite);
        countAlignment2 = AlignmentChecker::countDirection(pos, {-dx, -dy}, 5, grid, gridOpposite);
        int temp = result + countAlignment1.count + countAlignment2.count;
        if (temp > result) {
            result = temp;
            blocked = countAlignment1.blocked + countAlignment2.blocked;
        }
    }

    if (result < 2) {
        return result;
    }
    switch (blocked) {
        case 1:
            result *= 20;
            break;
        case 2:
            result *= 10;
            break;
        default:
            result *= 40;
            break;
    }

    return result;
}

int EvaluationService::checlAllAlignmentOpp(Board board, Position pos, const bool color) {
    Board::StoneMask grid = color ? board.getGridWhite() : board.getGridBlack();
    Board::StoneMask opposite = color ? board.getGridBlack() : board.getGridWhite();

    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };
    for (auto& [dx, dy] : directions) {
        AlignmentChecker::Result result1 = AlignmentChecker::countDirection(pos, {dx, dy}, 3, opposite, grid);
        AlignmentChecker::Result result2 = AlignmentChecker::countDirection(pos, {-dx, -dy}, 3, opposite, grid);
        if (result1.count + result2.count >= 3) {
            switch (result1.blocked + result2.blocked) {
                case 1:
                    return 1000;
                case 2:
                    std::cout << "blocked" << std::endl;
                    return 2000;
                default:
                    std::cout << "free" << std::endl;
                    return 5000;
            }
        }
    }
}
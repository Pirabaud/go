#include "FastEvaluator.hpp"
#include "AlignmentChecker.hpp"
#include "CheckWinService.hpp"

int FastEvaluator::evaluate(const Board& board) {
    const sf::Color* winner = CheckWinService::isWin(const_cast<Board&>(board));
    if (winner) {
        return (*winner == sf::Color::White) ? WIN_SCORE : -WIN_SCORE;
    }

    int whiteCaptured = board.getWhiteCaptured();
    int blackCaptured = board.getBlackCaptured();

    if (blackCaptured >= 10) return WIN_SCORE;
    if (whiteCaptured >= 10) return -WIN_SCORE;

    return evaluateCaptures(board);
}

int FastEvaluator::evaluateMove(Board& board, Position move, bool isWhite) {
    int score = 0;

    auto undo = board.makeMove(move, isWhite);

    int captureCount = undo.capturedStones.size() / 2;
    if (captureCount > 0) {
        score += captureCount * IMMEDIATE_CAPTURE;
    }

    const Board::StoneMask& grid = isWhite ? board.getGridWhite() : board.getGridBlack();
    const Board::StoneMask& oppGrid = isWhite ? board.getGridBlack() : board.getGridWhite();

    Alignment five = AlignmentChecker::detectAlignment(move, 5, grid, oppGrid);
    if (five != Alignment::NOTALIGN) {
        board.undoMove(undo);
        return WIN_SCORE;
    }

    Alignment four = AlignmentChecker::detectAlignment(move, 4, grid, oppGrid);
    if (four == Alignment::FREE) {
        score += FOUR_OPEN;
    } else if (four == Alignment::SEMIBLOCKED) {
        score += FOUR_SEMI;
    }

    Alignment three = AlignmentChecker::detectAlignment(move, 3, grid, oppGrid);
    if (three == Alignment::FREE) {
        score += THREE_OPEN;
    } else if (three == Alignment::SEMIBLOCKED) {
        score += THREE_SEMI;
    }

    Alignment two = AlignmentChecker::detectAlignment(move, 2, grid, oppGrid);
    if (two == Alignment::FREE) {
        score += TWO_OPEN;
    }

    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto& [dx, dy] : directions) {
        for (int sign = -1; sign <= 1; sign += 2) {
            int x1 = move.x + dx * sign;
            int y1 = move.y + dy * sign;
            int x2 = move.x + dx * sign * 2;
            int y2 = move.y + dy * sign * 2;
            int x3 = move.x + dx * sign * 3;
            int y3 = move.y + dy * sign * 3;

            if (x1 >= 0 && x1 < Board::SIZE && y1 >= 0 && y1 < Board::SIZE &&
                x2 >= 0 && x2 < Board::SIZE && y2 >= 0 && y2 < Board::SIZE &&
                x3 >= 0 && x3 < Board::SIZE && y3 >= 0 && y3 < Board::SIZE) {

                Position p1{x1, y1};
                Position p2{x2, y2};
                Position p3{x3, y3};

                if (!Board::isStoneAt(grid, p1) && !Board::isStoneAt(oppGrid, p1) &&
                    Board::isStoneAt(oppGrid, p2) && Board::isStoneAt(oppGrid, p3)) {
                    score += CAPTURE_THREAT;
                }
            }
        }
    }

    board.undoMove(undo);
    return isWhite ? score : -score;
}

bool FastEvaluator::canCapture(const Board& board, Position move, bool isWhite) {
    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto& [dx, dy] : directions) {
        int x1 = move.x + dx;
        int y1 = move.y + dy;
        int x2 = move.x + dx * 2;
        int y2 = move.y + dy * 2;
        int x3 = move.x + dx * 3;
        int y3 = move.y + dy * 3;

        if (x1 >= 0 && x1 < Board::SIZE && y1 >= 0 && y1 < Board::SIZE &&
            x2 >= 0 && x2 < Board::SIZE && y2 >= 0 && y2 < Board::SIZE &&
            x3 >= 0 && x3 < Board::SIZE && y3 >= 0 && y3 < Board::SIZE) {

            Position p1{x1, y1};
            Position p2{x2, y2};
            Position p3{x3, y3};

            bool enemy1 = isWhite ? board.isBlackStoneAt(p1) : board.isWhiteStoneAt(p1);
            bool enemy2 = isWhite ? board.isBlackStoneAt(p2) : board.isWhiteStoneAt(p2);
            bool ally3 = isWhite ? board.isWhiteStoneAt(p3) : board.isBlackStoneAt(p3);

            if (enemy1 && enemy2 && ally3) {
                return true;
            }
        }
    }

    return false;
}

int FastEvaluator::countCapturesForMove(const Board& board, Position move, bool isWhite) {
    int captureCount = 0;
    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    for (auto& [dx, dy] : directions) {
        int x1 = move.x + dx;
        int y1 = move.y + dy;
        int x2 = move.x + dx * 2;
        int y2 = move.y + dy * 2;
        int x3 = move.x + dx * 3;
        int y3 = move.y + dy * 3;

        if (x1 >= 0 && x1 < Board::SIZE && y1 >= 0 && y1 < Board::SIZE &&
            x2 >= 0 && x2 < Board::SIZE && y2 >= 0 && y2 < Board::SIZE &&
            x3 >= 0 && x3 < Board::SIZE && y3 >= 0 && y3 < Board::SIZE) {

            Position p1{x1, y1};
            Position p2{x2, y2};
            Position p3{x3, y3};

            bool enemy1 = isWhite ? board.isBlackStoneAt(p1) : board.isWhiteStoneAt(p1);
            bool enemy2 = isWhite ? board.isBlackStoneAt(p2) : board.isWhiteStoneAt(p2);
            bool ally3 = isWhite ? board.isWhiteStoneAt(p3) : board.isBlackStoneAt(p3);

            if (enemy1 && enemy2 && ally3) {
                captureCount++;
            }
        }
    }

    return captureCount;
}

bool FastEvaluator::isWinningPosition(const Board& board, bool isWhite) {
    const sf::Color* winner = CheckWinService::isWin(const_cast<Board&>(board));
    if (winner) {
        return (isWhite && *winner == sf::Color::White) || (!isWhite && *winner == sf::Color::Black);
    }

    int captured = isWhite ? board.getBlackCaptured() : board.getWhiteCaptured();
    return captured >= 10;
}

int FastEvaluator::evaluateCaptures(const Board& board) {
    int whiteCaptured = board.getWhiteCaptured();
    int blackCaptured = board.getBlackCaptured();

    int captureScore = (blackCaptured - whiteCaptured) * CAPTURE_PAIR;

    if (blackCaptured >= 8) captureScore += CAPTURE_WIN_SCORE / 2;
    if (whiteCaptured >= 8) captureScore -= CAPTURE_WIN_SCORE / 2;

    return captureScore;
}

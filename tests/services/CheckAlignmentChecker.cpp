#include <iostream>
#include "catch2/catch_test_macros.hpp"
#include "CheckMoveService.hpp"

struct BoardFixture {
    Board board;

    void fillDiagonal(const Position pos, int count, const Position dir, const bool color, const int changeColor) {
        int oppColor = 0;
        for (int k = 0; k <= count; k++) {
            if (pos.x + dir.x * k >= Board::SIZE
                || pos.x + dir.x * k < 0
                || pos.y + dir.y * k < 0
                || pos.y + dir.y * k >= Board::SIZE) {
                break;
            }
            if (oppColor == changeColor &&
                pos.y > 0
                && pos.x < Board::SIZE - 1) {
                addStoneAt(Position(pos.x + dir.x * k, pos.y + dir.y * k), !color);
                oppColor = 0;
           }
            else if (oppColor < changeColor) {
                oppColor++;
                addStoneAt(Position(pos.x + dir.x * k, pos.y + dir.y * k), color);
            }
        }
}

    void removeStoneAt(const Position pos, const bool color) {
        if (color) {
            board.removeBlackStoneAt(Position(pos.x, pos.y));
        }
        else {
            board.removeWhiteStoneAt(Position(pos.x, pos.y));
        }
    }

    void addStoneAt(const Position pos, const bool color) {
        if (color) {
            board.addStoneBlack(Position(pos.x, pos.y));
        }
        else {
            board.addStoneWhite(Position(pos.x, pos.y));
        }
    }

    void emptyDiagonal(const Position pos, int count, Position dir, bool color, int changeColor) {
        int oppColor = 0;
        for (int k = 0; k < count; k++) {
            if (pos.x + dir.x * k >= Board::SIZE
                         || pos.x + dir.x * k < 0
                         || pos.y + dir.y * k < 0
                         || pos.y + dir.y * k >= Board::SIZE) {
                break;
                         }
            if (oppColor > changeColor) {
                oppColor = 0;
                removeStoneAt(Position(pos.x + dir.x * k, pos.y + dir.y * k), !color);
            }
            else {
                oppColor++;
                removeStoneAt(Position(pos.x + dir.x * k, pos.y + dir.y * k), color);
            }
        }
    }
};

TEST_CASE_METHOD(BoardFixture, "Check not alignment line for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 0; j < Board::SIZE; j++) {
            if (oppColor > 1) {
                board.addStoneBlack(Position(i, j));
                oppColor = 0;
            }
            else {
                board.addStoneWhite(Position(i, j));
                REQUIRE(AlignmentChecker::detectAlignment(
               Position(i,j),
               3,
               board.getGridWhite(),
               board.getGridBlack()) == Alignment::NOTALIGN);
                oppColor++;
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment line for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 0; j < Board::SIZE; j++) {
            if (oppColor > 1) {
                board.addStoneWhite(Position(i, j));
                oppColor = 0;
            }
            else {
                REQUIRE(AlignmentChecker::detectAlignment(
               Position(i,j),
               3,
               board.getGridBlack(),
               board.getGridWhite()) == Alignment::NOTALIGN);
                oppColor++;
                board.addStoneBlack(Position(i, j));
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked line for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor=0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 0; j < Board::SIZE - 1; j++) {
            std::cout << j << " " << ;
            if (oppColor > 2) {
                board.addStoneBlack(Position(i, j));
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneWhite(Position(i, j));
            }
            if (oppColor == 3) {
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position(i,j - 1),
                    3,
                    board.getGridWhite(),
                    board.getGridBlack()) == Alignment::SEMIBLOCKED);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked line for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 0; j < Board::SIZE - 1; j++) {
            if (oppColor > 2) {
                board.addStoneWhite(Position(i, j));
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneBlack(Position(i, j));
            }
            if (oppColor == 3) {
                    REQUIRE(AlignmentChecker::detectAlignment(
                   Position(i,j),
                   3,
                   board.getGridBlack(),
                   board.getGridWhite()) == Alignment::SEMIBLOCKED);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked line for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 0; j < Board::SIZE; j++) {
            if (oppColor > 2) {
                board.addStoneBlack(Position(i, j));
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneWhite(Position(i, j));
            }
            if (oppColor == 0 && j > 0) {
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position(i,j - 1),
                    3,
                    board.getGridWhite(),
                    board.getGridBlack()) == Alignment::BLOCKED);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked line for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 0; j < Board::SIZE; j++) {
            if (oppColor > 2) {
                board.addStoneWhite(Position(i, j));
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneBlack(Position(i, j));
            }
            if (oppColor == 0 && j > 0) {
                    REQUIRE(AlignmentChecker::detectAlignment(
                   Position(i,j - 1),
                   3,
                   board.getGridBlack(),
                   board.getGridWhite()) == Alignment::BLOCKED);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free line for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 1; j < Board::SIZE; j++) {
            if (oppColor > 2) {
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneWhite(Position(i, j));
            }
            if (oppColor == 0 && j > 1) {
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position(i,j),
                    3,
                    board.getGridWhite(),
                    board.getGridBlack()) == Alignment::FREE);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free line for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 1; j < Board::SIZE; j++) {
            if (oppColor > 2) {
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneBlack(Position(i, j));
            }
            if (oppColor == 0 && j > 1) {
                    REQUIRE(AlignmentChecker::detectAlignment(
                   Position(i,j),
                   3,
                   board.getGridBlack(),
                   board.getGridWhite()) == Alignment::FREE);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment col for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 0; j < Board::SIZE; j++) {
            if (oppColor > 1) {
                board.addStoneBlack(Position(j, i));
                oppColor = 0;
            }
            else {
                board.addStoneWhite(Position(j, i));
                REQUIRE(AlignmentChecker::detectAlignment(
               Position(j,i),
               3,
               board.getGridWhite(),
               board.getGridBlack()) == Alignment::NOTALIGN);
                oppColor++;
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment col for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 0; j < Board::SIZE; j++) {
            if (oppColor > 1) {
                board.addStoneWhite(Position(j, i));
                oppColor = 0;
            }
            else {
                REQUIRE(AlignmentChecker::detectAlignment(
               Position(j,i),
               3,
               board.getGridBlack(),
               board.getGridWhite()) == Alignment::NOTALIGN);
                oppColor++;
                board.addStoneBlack(Position(j, i));
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked col for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 0; j < Board::SIZE - 1; j++) {
            if (oppColor > 2) {
                board.addStoneBlack(Position(j, i));
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneWhite(Position(j, i));
            }
            if (oppColor == 3) {
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position(j,i),
                    3,
                    board.getGridWhite(),
                    board.getGridBlack()) == Alignment::SEMIBLOCKED);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked col for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 0; j < Board::SIZE - 1; j++) {
            if (oppColor > 2) {
                board.addStoneWhite(Position(j, i));
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneBlack(Position(j, i));
            }
            if (oppColor == 3) {
                    REQUIRE(AlignmentChecker::detectAlignment(
                   Position(j,i),
                   3,
                   board.getGridBlack(),
                   board.getGridWhite()) == Alignment::SEMIBLOCKED);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked col for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 0; j < Board::SIZE; j++) {
            if (oppColor > 2) {
                board.addStoneBlack(Position(j, i));
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneWhite(Position(j, i));
            }
            if (oppColor == 0 && j > 0) {
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position(j - 1,i),
                    3,
                    board.getGridWhite(),
                    board.getGridBlack()) == Alignment::BLOCKED);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked col for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 0; j < Board::SIZE; j++) {
            if (oppColor > 2) {
                board.addStoneWhite(Position(j, i));
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneBlack(Position(j, i));
            }
            if (oppColor == 0 && j > 0) {
                    REQUIRE(AlignmentChecker::detectAlignment(
                   Position(j - 1, i),
                   3,
                   board.getGridBlack(),
                   board.getGridWhite()) == Alignment::BLOCKED);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free col for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 1; j < Board::SIZE; j++) {
            if (oppColor > 2) {
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneWhite(Position(i, j));
            }
            if (oppColor == 0 && j > 1) {
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position(i,j),
                    3,
                    board.getGridWhite(),
                    board.getGridBlack()) == Alignment::FREE);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free col for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 1; j < Board::SIZE; j++) {
            if (oppColor > 2) {
                oppColor = 0;
            }
            else {
                oppColor++;
                board.addStoneBlack(Position(i, j));
            }
            if (oppColor == 0 && j > 1) {
                    REQUIRE(AlignmentChecker::detectAlignment(
                   Position(i,j),
                   3,
                   board.getGridBlack(),
                   board.getGridWhite()) == Alignment::FREE);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment diag top right white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 0; j < Board::SIZE; j += 4) {
            fillDiagonal(Position(i, j), 3, Position(-1, 1), false, 1);
            REQUIRE(AlignmentChecker::detectAlignment(
              Position(i,j),
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::NOTALIGN);
            emptyDiagonal(Position(i, j), 3, Position(-1, 1), false, 1);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment diag top right white test") {
    fillDiagonal(Position(2, 0), 3, Position(-1, 1), false, 1);
    REQUIRE(AlignmentChecker::detectAlignment(
              Position(2,0),
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::NOTALIGN);
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment diag top right black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE; j += 4) {
            fillDiagonal(Position(i, j), 3, Position(1, 1), true, 1);
            REQUIRE(AlignmentChecker::detectAlignment(
              Position(i,j),
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::NOTALIGN);
            emptyDiagonal(Position(i, j), 3, Position(1, 1), true, 1);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked diag top right white") {
    for (int i = 4; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE - 4; j += 4) {
            fillDiagonal(Position(i, j), 4, Position(-1, 1), false, 4);
            REQUIRE(AlignmentChecker::detectAlignment(
              Position(i,j),
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::SEMIBLOCKED);
            emptyDiagonal(Position(i, j), 4, Position(-1, 1), false, 4);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked diag top right black") {
    for (int i = 4; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE - 4; j += 4) {
            std::cout << i << " " << j << std::endl;
            fillDiagonal(Position(i, j), 3, Position(-1, 1), true, 3);
            REQUIRE(AlignmentChecker::detectAlignment(
              Position(i,j),
              3,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::SEMIBLOCKED);
            emptyDiagonal(Position(i, j), 3, Position(-1, 1), true, 3);
        }
    }
}
#include <bitset>
#include <iostream>
#include "catch2/catch_test_macros.hpp"
#include "CheckMoveService.hpp"
#include "catch2/internal/catch_stdstreams.hpp"

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
            if (oppColor == changeColor) {
                std::cout << k << " : " << (color ? "white : " : "black : ") << pos.x + dir.x * k << " " << pos.y + dir.y * k << std::endl;
                addStoneAt (Position{pos.x + dir.x * k, pos.y + dir.y * k}, !color);
                oppColor = 0;
           }
            else if (oppColor <= changeColor) {
                oppColor++;
                std::cout << k << " : " << (color ? "black : " : "white : ") << pos.x + dir.x * k << " " << pos.y + dir.y * k << std::endl;
                addStoneAt (Position{pos.x + dir.x * k, pos.y + dir.y * k}, color);
            }
        }
}

    void removeStoneAt(const Position pos, const bool color) {
        if (color) {
            board.removeBlackStoneAt (Position{pos.x, pos.y});
        }
        else {
            board.removeWhiteStoneAt (Position{pos.x, pos.y});
        }
    }

    void addStoneAt(const Position pos, const bool color) {
        if (color) {
            board.addStoneBlack (Position{pos.x, pos.y});
        }
        else {
            board.addStoneWhite (Position{pos.x, pos.y});
        }
    }

    void emptyDiagonal(const Position pos, int count, Position dir, bool color, int changeColor) {
        int oppColor = 0;
        for (int k = 0; k <= count; k++) {
            if (pos.x + dir.x * k >= Board::SIZE
                         || pos.x + dir.x * k < 0
                         || pos.y + dir.y * k < 0
                         || pos.y + dir.y * k >= Board::SIZE) {
                break;
                         }
            if (oppColor >= changeColor) {
                oppColor = 0;
                removeStoneAt (Position{pos.x + dir.x * k, pos.y + dir.y * k}, !color);
            }
            else {
                oppColor++;
                removeStoneAt (Position{pos.x + dir.x * k, pos.y + dir.y * k}, color);
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
            oppColor++;
            if (oppColor < 3) {
                board.addStoneWhite (Position{i, j});
                std::cout << "white : " << i << " " << j << std::endl;
            }
            else {
                std::cout << "test : " << i << " " << j << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
               Position{i, j},
               4,
               board.getGridWhite(),
               board.getGridBlack()) == Alignment::NOTALIGN);
                oppColor = 0;
            }
            if (oppColor == 0 && j + 1 < Board::SIZE) {;
                j++;
                std::cout << "black : " << i << " " << j << std::endl;
                board.addStoneBlack (Position{i, j});
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
            oppColor++;
            if (oppColor < 3) {
                board.addStoneBlack (Position{i, j});
                std::cout << "black : " << i << " " << j << std::endl;
            }
            else {
                std::cout << "test : " << i << " " << j << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
               Position{i, j},
               4,
               board.getGridBlack(),
               board.getGridWhite()) == Alignment::NOTALIGN);
                oppColor = 0;
            }
            if (oppColor == 0 && j + 1 < Board::SIZE) {;
                j++;
                std::cout << "white : " << i << " " << j << std::endl;
                board.addStoneWhite (Position{i, j});
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked right line for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor = 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 1; j < Board::SIZE - 1; j++) {
            oppColor++;
            if (oppColor <= 3){
                std::cout << "White : " << i << " " << j << std::endl;
                board.addStoneWhite (Position{i, j});
            }
            else {
                oppColor = 0;
                std::cout << "black : " << i << " " << j << std::endl;
                board.addStoneBlack (Position{i, j});
            }
            if (oppColor == 0 && j > 0) {
                std::cout << "test : " << i << " " << j - 1 << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{i, j - 1},
                    3,
                    board.getGridWhite(),
                    board.getGridBlack()) == Alignment::SEMIBLOCKED);
                j++;
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked left line for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor = 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 0; j < Board::SIZE - 1; j++) {
            oppColor++;
            if (oppColor < 3){
                std::cout << "White : " << i << " " << j << std::endl;
                board.addStoneWhite (Position{i, j});
            }
            else {
                std::cout << "test : " << i << " " << j << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{i, j},
                    3,
                    board.getGridWhite(),
                    board.getGridBlack()) == Alignment::SEMIBLOCKED);
                oppColor = 0;
            }
            if (oppColor == 0 && j + 1 < Board::SIZE) {
                std::cout << "black : " << i << " " << j + 1<< std::endl;
                j++;
                board.addStoneBlack (Position{i, j});
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked right line for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor = 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 1; j < Board::SIZE - 1; j++) {
            oppColor++;
            if (oppColor <= 3){
                std::cout << "Black : " << i << " " << j << std::endl;
                board.addStoneBlack (Position{i, j});
            }
            else {
                oppColor = 0;
                std::cout << "White : " << i << " " << j << std::endl;
                board.addStoneWhite (Position{i, j});
            }
            if (oppColor == 0 && j > 0) {
                std::cout << "test : " << i << " " << j - 1 << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{i, j - 1},
                    3,
                    board.getGridBlack(),
                    board.getGridWhite()) == Alignment::SEMIBLOCKED);
                j++;
            }

        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked left line for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor = 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 0; j < Board::SIZE - 1; j++) {
            oppColor++;
            if (oppColor < 3){
                std::cout << "black : " << i << " " << j << std::endl;
                board.addStoneBlack (Position{i, j});
            }
            else {
                std::cout << "test : " << i << " " << j << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{i, j},
                    3,
                    board.getGridBlack(),
                    board.getGridWhite()) == Alignment::SEMIBLOCKED);
                oppColor = 0;
            }
            if (oppColor == 0 && j + 1 < Board::SIZE) {
                j++;
                std::cout << "white : " << i << " " << j << std::endl;
                board.addStoneWhite (Position{i, j});
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
                std::cout << "black : " << i << " " << j << std::endl;
                board.addStoneBlack (Position{i, j});
                oppColor = 0;
            }
            else {
                oppColor++;
                std::cout << "white : " << i << " " << j << std::endl;
                board.addStoneWhite (Position{i, j});
            }
            if (oppColor == 0 && j > 0) {
                std::cout << "test : " << i << " " << j - 1 << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{i, j - 1},
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
                std::cout << "white : " << i << " " << j << std::endl;
                board.addStoneWhite (Position{i, j});
                oppColor = 0;
            }
            else {
                oppColor++;
                std::cout << "black : " << i << " " << j << std::endl;
                board.addStoneBlack (Position{i, j});
            }
            if (oppColor == 0 && j > 0) {
                std::cout << "test : " << i << " " << j - 1 << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{i, j - 1},
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
        for (int j = 1; j < Board::SIZE - 1; j++) {
            if (oppColor > 1) {
                oppColor = 0;
            }
            else {
                oppColor++;
                std::cout << "white : " << i << " " << j << std::endl;
                board.addStoneWhite (Position{i, j});
            }
            if (oppColor == 0 && j > 1) {
                std::cout << "test : " << i << " " << j << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{i, j},
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
        for (int j = 1; j < Board::SIZE - 1; j++) {
            if (oppColor > 1) {
                oppColor = 0;
            }
            else {
                oppColor++;
                std::cout << "black : " << i << " " << j << std::endl;
                board.addStoneBlack (Position{i, j});
            }
            if (oppColor == 0 && j > 1) {
                std::cout << "test : " << i << " " << j << std::endl;
                    REQUIRE(AlignmentChecker::detectAlignment(
                   Position{i, j},
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
            board.emptyColumn(i - 1);
        }
        for (int j = 0; j < Board::SIZE; j++) {
            oppColor++;
            if (oppColor < 3) {
                board.addStoneWhite (Position{j, i});
                std::cout << "white : " << j << " " << i << std::endl;
            }
            else {
                std::cout << "test : " << j << " " << i << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
               Position{j, i},
               4,
               board.getGridWhite(),
               board.getGridBlack()) == Alignment::NOTALIGN);
                oppColor = 0;
            }
            if (oppColor == 0 && j + 1 < Board::SIZE) {;
                j++;
                std::cout << "black : " << j << " " << i << std::endl;
                board.addStoneBlack (Position{j, i});
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment col for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor= 0;
        if (i > 0) {
            board.emptyColumn(i - 1);
        }
        for (int j = 0; j < Board::SIZE; j++) {
            oppColor++;
            if (oppColor < 3) {
                board.addStoneBlack (Position{j, i});
                std::cout << "black : " << j << " " << i << std::endl;
            }
            else {
                std::cout << "test : " << j << " " << i << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
               Position{j, i},
               4,
               board.getGridBlack(),
               board.getGridWhite()) == Alignment::NOTALIGN);
                oppColor = 0;
            }
            if (oppColor == 0 && j + 1 < Board::SIZE) {;
                j++;
                std::cout << "white : " << j << " " << i << std::endl;
                board.addStoneWhite (Position{j, i});
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked down col for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor = 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 1; j < Board::SIZE - 1; j++) {
            oppColor++;
            if (oppColor <= 3){
                std::cout << "White : " << j << " " << i << std::endl;
                board.addStoneWhite (Position{j, i});
            }
            else {
                oppColor = 0;
                std::cout << "black : " << j << " " << i << std::endl;
                board.addStoneBlack (Position{j, i});
            }
            if (oppColor == 0 && j > 0) {
                std::cout << "test : " << j - 1 << " " << i << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{j - 1, i},
                    3,
                    board.getGridWhite(),
                    board.getGridBlack()) == Alignment::SEMIBLOCKED);
                j++;
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked up col for white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor = 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 0; j < Board::SIZE - 1; j++) {
            oppColor++;
            if (oppColor < 3){
                std::cout << "White : " << j << " " << i << std::endl;
                board.addStoneWhite (Position{j, i});
            }
            else {
                std::cout << "test : " << j << " " << i << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{j, i},
                    3,
                    board.getGridWhite(),
                    board.getGridBlack()) == Alignment::SEMIBLOCKED);
                oppColor = 0;
            }
            if (oppColor == 0 && j + 1 < Board::SIZE) {
                j++;
                std::cout << "black : " << j << " " << i<< std::endl;
                board.addStoneBlack (Position{j, i});
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked down col for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor = 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 1; j < Board::SIZE - 1; j++) {
            oppColor++;
            if (oppColor <= 3){
                std::cout << "Black : " << j << " " << i << std::endl;
                board.addStoneBlack (Position{j, i});
            }
            else {
                oppColor = 0;
                std::cout << "White : " << j << " " << i << std::endl;
                board.addStoneWhite (Position{j, i});
            }
            if (oppColor == 0 && j > 0) {
                std::cout << "test : " << j - 1 << " " << i << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{j - 1, i},
                    3,
                    board.getGridBlack(),
                    board.getGridWhite()) == Alignment::SEMIBLOCKED);
                j++;
            }

        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked up col for black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        int oppColor = 0;
        if (i > 0) {
            board.emptyLine(i - 1);
        }
        for (int j = 0; j < Board::SIZE - 1; j++) {
            oppColor++;
            if (oppColor < 3){
                std::cout << "black : " << j << " " << i << std::endl;
                board.addStoneBlack (Position{j, i});
            }
            else {
                std::cout << "test : " << j << " " << i << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{j, i},
                    3,
                    board.getGridBlack(),
                    board.getGridWhite()) == Alignment::SEMIBLOCKED);
                oppColor = 0;
            }
            if (oppColor == 0 && j + 1 < Board::SIZE) {
                j++;
                std::cout << "white : " << j << " " << i << std::endl;
                board.addStoneWhite (Position{j, i});
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
                std::cout << "black : " << j << " " << i << std::endl;
                board.addStoneBlack (Position{j, i});
                oppColor = 0;
            }
            else {
                oppColor++;
                std::cout << "white : " << j << " " << i << std::endl;
                board.addStoneWhite (Position{j, i});
            }
            if (oppColor == 0 && j > 0) {
                std::cout << "test : " << j - 1 << " " << i << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{j - 1, i},
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
                std::cout << "white : " << j << " " << i << std::endl;
                board.addStoneWhite (Position{j, i});
                oppColor = 0;
            }
            else {
                oppColor++;
                std::cout << "black : " << j << " " << i << std::endl;
                board.addStoneBlack (Position{j, i});
            }
            if (oppColor == 0 && j > 0) {
                std::cout << "test : " << j << " " << i << std::endl;
                    REQUIRE(AlignmentChecker::detectAlignment(
                   Position{j - 1, i},
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
        for (int j = 1; j < Board::SIZE - 1; j++) {
            if (oppColor > 1) {
                oppColor = 0;
            }
            else {
                oppColor++;
                std::cout << "white : " << j << " " << i << std::endl;
                board.addStoneWhite (Position{i, j});
            }
            if (oppColor == 0) {
                std::cout << "test : " << j << " " << i << std::endl;
                REQUIRE(AlignmentChecker::detectAlignment(
                    Position{i, j},
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
        for (int j = 1; j < Board::SIZE - 1; j++) {
            if (oppColor > 1) {
                oppColor = 0;
            }
            else {
                std::cout << "black : " << j << " " << i << std::endl;
                oppColor++;
                board.addStoneBlack (Position{i, j});
            }
            if (oppColor == 0 && j > 1) {
                std::cout << "test : " << j << " " << i << std::endl;
                    REQUIRE(AlignmentChecker::detectAlignment(
                   Position{i, j},
                   3,
                   board.getGridBlack(),
                   board.getGridWhite()) == Alignment::FREE);
            }
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment diag top right white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{-1, 1}, false, 2);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::NOTALIGN);
            emptyDiagonal (Position{i, j}, 3, Position{-1, 1}, false, 2);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment diag top right black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{-1, 1}, true, 2);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::NOTALIGN);
            emptyDiagonal (Position{i, j}, 3, Position{-1, 1}, true, 2);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked right diag top right white") {
    for (int i = 4; i < Board::SIZE - 1; i++) {
        for (int j = 1; j < Board::SIZE - 4; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{-1, 1}, false, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, 1}, false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked right diag top right black") {
    for (int i = 4; i < Board::SIZE - 1; i++) {
        for (int j = 1; j < Board::SIZE - 4; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{-1, 1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, 1}, true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked left diag top right white") {
    for (int i = 4; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE - 4; j += 4) {
            if (!CheckMoveService::notInBoard (Position{i + 1, j - 1})) {
                std::cout << "black : " << i + 1 << " " << j - 1 << std::endl;
                board.addStoneBlack (Position{i + 1, j - 1});
            }
                fillDiagonal (Position{i, j}, 3, Position{-1, 1}, false, 4);

            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              4,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, 1}, false, 4);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked left diag top right black") {
    for (int i = 4; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE - 4; j += 4) {
            if (!CheckMoveService::notInBoard (Position{i + 1, j - 1})) {
                std::cout << "white : " << i + 1 << " " << j - 1 << std::endl;
                board.addStoneWhite (Position{i + 1, j - 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{-1, 1}, true, 4);

            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              4,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, 1}, true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked diag top right white") {
    for (int i = 4; i < Board::SIZE - 1 ; i++) {
        for (int j = 0; j < Board::SIZE - 4; j += 4) {
            if (j > 0) {
                std::cout << "black : " << i + 1 << " " << j - 1 << std::endl;
                board.addStoneBlack (Position{i + 1, j - 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{-1, 1}, false, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::BLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, 1}, false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked diag top right black") {
    for (int i = 4; i < Board::SIZE - 1 ; i++) {
        for (int j = 0; j < Board::SIZE - 4; j += 4) {
            if (j > 0) {
                std::cout << "white : " << i + 1 << " " << j - 1 << std::endl;
                board.addStoneWhite (Position{i + 1, j - 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{-1, 1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::BLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, 1}, true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free diag top right white") {
    for (int i = 4; i < Board::SIZE - 1 ; i++) {
        for (int j = 1; j < Board::SIZE - 4; j += 4) {
            fillDiagonal (Position{i, j}, 2, Position{-1, 1}, false, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(
              AlignmentChecker::detectAlignment(
                Position{i, j},
                3,
                board.getGridWhite(),
                board.getGridBlack()
              ) == Alignment::FREE );
            emptyDiagonal (Position{i, j}, 2, Position{-1, 1},false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free diag top right black") {
    for (int i = 4; i < Board::SIZE - 1 ; i++) {
        for (int j = 1; j < Board::SIZE - 4; j += 4) {
            fillDiagonal (Position{i, j}, 2, Position{-1, 1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(
              AlignmentChecker::detectAlignment(
                Position{i, j},
                3,
                board.getGridBlack(),
                board.getGridWhite()
              ) == Alignment::FREE );
            emptyDiagonal (Position{i, j}, 2, Position{-1, 1},true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment diag top left white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{-1, -1}, true, 1);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::NOTALIGN);
            emptyDiagonal (Position{i, j}, 3, Position{-1, -1}, true, 1);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment diag top left black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{-1, -1}, true, 1);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::NOTALIGN);
            emptyDiagonal (Position{i, j}, 3, Position{-1, -1}, true, 1);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked left diag top left white") {
    for (int i = Board::SIZE - 2; i >= 4 ; i--) {
        for (int j = Board::SIZE - 4; j >= 4; j -= 4) {
            fillDiagonal (Position{i, j}, 3, Position{-1, -1}, false, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, -1}, false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked left diag top left black") {
    for (int i = Board::SIZE - 2; i >= 4 ; i--) {
        for (int j = Board::SIZE - 4; j >= 4; j -= 4) {
            fillDiagonal (Position{i, j}, 3, Position{-1, -1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, -1}, true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked right diag top left white") {
    for (int i = Board::SIZE - 1; i >= 4  ; i--) {
        for (int j = Board::SIZE - 1; j >= 4; j -= 4) {
            if (!CheckMoveService::notInBoard (Position{i + 1, j + 1})) {
                std::cout << "black : " << i + 1 << " " << j + 1 << std::endl;
                board.addStoneBlack (Position{i + 1, j + 1});
            }
                fillDiagonal (Position{i, j}, 3, Position{-1, -1}, false, 4);

            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, -1}, false, 4);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked right diag top left black") {
    for (int i = Board::SIZE - 1; i >= 4  ; i--) {
        for (int j = Board::SIZE - 1; j >= 4; j -= 4) {
            if (!CheckMoveService::notInBoard (Position{i + 1, j + 1})) {
                std::cout << "white : " << i + 1 << " " << j + 1 << std::endl;
                board.addStoneWhite (Position{i + 1, j + 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{-1, -1}, true, 4);

            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, -1}, true, 4);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked diag top left white") {
    for (int i = Board::SIZE - 1; i >= 4  ; i--) {
        for (int j = Board::SIZE - 1; j >= 4; j -= 4) {
            if (i < Board::SIZE - 1) {
                std::cout << "black : " << i + 1 << " " << j + 1 << std::endl;
                board.addStoneBlack (Position{i + 1, j + 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{-1, -1}, false, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::BLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, -1}, false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked diag top left black") {
    for (int i = Board::SIZE - 1; i >= 4  ; i--) {
        for (int j = Board::SIZE - 1; j >= 4; j -= 4) {
            if (i < Board::SIZE - 1) {
                std::cout << "white : " << i + 1 << " " << j + 1 << std::endl;
                board.addStoneWhite (Position{i + 1, j + 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{-1, -1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::BLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{-1, -1}, true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free diag top left white") {
    for (int i = Board::SIZE - 2; i >= 4  ; i--) {
        for (int j = Board::SIZE - 2; j >= 4; j -= 4) {
            fillDiagonal (Position{i, j}, 2, Position{-1, -1}, false, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(
              AlignmentChecker::detectAlignment(
                Position{i, j},
                3,
                board.getGridWhite(),
                board.getGridBlack()
              ) == Alignment::FREE );
            emptyDiagonal (Position{i, j}, 2, Position{-1, -1},false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free diag top left black") {
    for (int i = Board::SIZE - 2; i >= 4  ; i--) {
        for (int j = Board::SIZE - 2; j >= 4; j -= 4) {
            fillDiagonal (Position{i, j}, 2, Position{-1, -1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(
              AlignmentChecker::detectAlignment(
                Position{i, j},
                3,
                board.getGridBlack(),
                board.getGridWhite()
              ) == Alignment::FREE );
            emptyDiagonal (Position{i, j}, 2, Position{-1, -1},true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment diag down left white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{1, -1}, false, 1);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::NOTALIGN);
            emptyDiagonal (Position{i, j}, 3, Position{1, -1}, false, 1);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment diag down left black") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{1, -1}, true, 1);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::NOTALIGN);
            emptyDiagonal (Position{i, j}, 3, Position{1, -1}, true, 1);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked left diag down left white") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = Board::SIZE - 2; j > 4; j -= 4) {
            fillDiagonal (Position{i, j}, 3, Position{1, -1}, false, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, -1}, false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked left diag down left black") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = Board::SIZE - 2; j > 4; j -= 4) {
            fillDiagonal (Position{i, j}, 3, Position{1, -1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, -1}, true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked right diag down left white") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = Board::SIZE - 2; j > 4; j -= 4) {
            if (!CheckMoveService::notInBoard (Position{i - 1, j + 1})) {
                std::cout << "black : " << i - 1 << " " << j + 1 << std::endl;
                board.addStoneBlack (Position{i - 1, j + 1});
            }
                fillDiagonal (Position{i, j}, 3, Position{1, -1}, false, 4);

            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, -1}, false, 4);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked right diag down left black") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = Board::SIZE - 2; j > 4; j -= 4) {
            if (!CheckMoveService::notInBoard (Position{i - 1, j + 1})) {
                std::cout << "white : " << i + 1 << " " << j - 1 << std::endl;
                board.addStoneWhite (Position{i - 1, j + 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{1, -1}, true, 4);

            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, -1}, true, 4);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked diag down left white") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = Board::SIZE - 2; j > 4; j -= 4) {
            if (j > 0) {
                std::cout << "black : " << i - 1 << " " << j + 1 << std::endl;
                board.addStoneBlack (Position{i - 1, j + 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{1, -1}, false, 3);
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::BLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, -1}, false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked diag down left black") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = Board::SIZE - 2; j > 4; j -= 4) {
            if (j > 0) {
                std::cout << "white : " << i - 1 << " " << j + 1 << std::endl;
                board.addStoneWhite (Position{i - 1, j + 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{1, -1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::BLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, - 1}, true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free diag down left white") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = Board::SIZE - 2; j > 4; j -= 4) {
            fillDiagonal (Position{i, j}, 2, Position{1, -1}, false, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(
              AlignmentChecker::detectAlignment(
                Position{i, j},
                3,
                board.getGridWhite(),
                board.getGridBlack()
              ) == Alignment::FREE );
            emptyDiagonal (Position{i, j}, 2, Position{1, -1},false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free diag down left black") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = Board::SIZE - 2; j > 4; j -= 4) {
            fillDiagonal (Position{i, j}, 2, Position{1, -1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(
              AlignmentChecker::detectAlignment(
                Position{i, j},
                3,
                board.getGridBlack(),
                board.getGridWhite()
              ) == Alignment::FREE );
            emptyDiagonal (Position{i, j}, 2, Position{1, -1},true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment diag down right white") {
    for (int i = 0 ; i < Board::SIZE ; i++) {
        for (int j = 0; j < Board::SIZE; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{1, 1}, false, 1);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::NOTALIGN);
            emptyDiagonal (Position{i, j}, 3, Position{1, 1}, false, 1);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check not alignment diag down right black") {
    for (int i = 0; i < Board::SIZE; i++) {
        for (int j = 0; j < Board::SIZE; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{1, 1}, true, 1);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::NOTALIGN);
            emptyDiagonal (Position{i, j}, 3, Position{1, 1}, true, 1);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked right diag down right white") {
    for (int i = 4; i < Board::SIZE - 4; i++) {
        for (int j = 1; j < Board::SIZE - 4; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{1, 1}, false, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, 1}, false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked right diag down right black") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = 1; j < Board::SIZE - 4; j += 4) {
            fillDiagonal (Position{i, j}, 3, Position{1, 1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
                Position{i, j},
                3,
                board.getGridBlack(),
                board.getGridWhite()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, 1}, true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked left diag down right white") {
    for (int i = 4; i < Board::SIZE - 4; i++) {
        for (int j = 1; j < Board::SIZE - 4; j += 4) {
            if (!CheckMoveService::notInBoard (Position{i - 1, j - 1})) {
                std::cout << "black : " << i - 1 << " " << j - 1 << std::endl;
                board.addStoneBlack (Position{i - 1, j - 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{1, 1}, false, 4);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              4,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, 1}, false, 4);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment semi-blocked left diag down right black") {
    for (int i = 4; i < Board::SIZE - 4; i++) {
        for (int j = 1; j < Board::SIZE - 4; j += 4) {
            if (!CheckMoveService::notInBoard (Position{i - 1, j - 1})) {
                std::cout << "white : " << i - 1 << " " << j - 1 << std::endl;
                board.addStoneWhite (Position{i - 1, j - 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{1, 1}, true, 4);

            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::SEMIBLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, 1}, true, 4);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked diag down right white") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = 0; j < Board::SIZE - 4; j += 4) {
            if (j > 0) {
                std::cout << "black : " << i - 1 << " " << j - 1 << std::endl;
                board.addStoneBlack (Position{i - 1, j - 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{1, 1}, false, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridWhite(),
              board.getGridBlack()) == Alignment::BLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, 1}, false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment blocked diag down right black") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = 0; j < Board::SIZE - 4; j += 4) {
            if (j > 0) {
                std::cout << "white : " << i - 1 << " " << j - 1 << std::endl;
                board.addStoneWhite (Position{i - 1, j - 1});
            }
            fillDiagonal (Position{i, j}, 3, Position{1, 1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(AlignmentChecker::detectAlignment(
              Position{i, j},
              3,
              board.getGridBlack(),
              board.getGridWhite()) == Alignment::BLOCKED);
            emptyDiagonal (Position{i, j}, 3, Position{1, 1}, true, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free diag down right white") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = 1; j < Board::SIZE - 4; j += 4) {
            fillDiagonal (Position{i, j}, 2, Position{-1, 1}, false, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(
              AlignmentChecker::detectAlignment(
                Position{i, j},
                3,
                board.getGridWhite(),
                board.getGridBlack()
              ) == Alignment::FREE );
            emptyDiagonal (Position{i, j}, 2, Position{-1, 1},false, 3);
        }
    }
}

TEST_CASE_METHOD(BoardFixture, "Check alignment free diag down right black") {
    for (int i = 4; i < Board::SIZE - 4 ; i++) {
        for (int j = 1; j < Board::SIZE - 4; j += 4) {
            fillDiagonal (Position{i, j}, 2, Position{1, 1}, true, 3);
            std::cout << "test : " << i << " " << j << std::endl;
            REQUIRE(
              AlignmentChecker::detectAlignment(
                Position{i, j},
                3,
                board.getGridBlack(),
                board.getGridWhite()
              ) == Alignment::FREE );
            emptyDiagonal (Position{i, j}, 2, Position{1, 1},true, 3);
        }
    }
}

#include "CheckLegalMove.hpp"
#include "CaptureService.hpp"


IllegalMoves::Type CheckLegalMove::isLegalMove(int posIndex,
                                               Board& board,
                                               const bool& isBlack) {
    if (posIndex < 0 || posIndex > (Board::SIZE + 1) * Board::SIZE || (posIndex % (Board::SIZE + 1)) == Board::SIZE) {
        return IllegalMoves::Type::NOT_IN_BOARD;
    }
    if (Board::isBitAt(board.getBitBoardBlack(), posIndex) || Board::isBitAt(board.getBitBoardWhite(), posIndex)) {
        return IllegalMoves::Type::OCCUPIED;
    }
    if (createsDoubleFreeThree(posIndex, board, isBlack)) {
        return IllegalMoves::Type::DOUBLE_FREE_CAPTURE;
    }
    if (createsAutoCapture(posIndex, board, isBlack)) {
        return IllegalMoves::Type::CREATE_CAPTURE;
    }
    return IllegalMoves::Type::NONE;
}

bool CheckLegalMove::createsAutoCapture(int posIndex, Board& board, const bool& isBlack) {
    const std::array<int, 4> directions = {1, Board::SIZE + 1, Board::SIZE, Board::SIZE + 2};

    for (const int dir : directions) {
        if (checkCaptureInDirection(board, posIndex, dir, isBlack)) {
            return true;
        }
        if (checkCaptureInDirection(board, posIndex, -dir, isBlack)) {
            return true;
        }
    }
    return false;
}

bool CheckLegalMove::checkCaptureInDirection(Board& board, const int globalIndex,
                                             const int dir, const bool isBlack) {
    if (globalIndex - 1 * dir < 0 || globalIndex - 1 * dir >= 380 || Board::isOutOfBounds(globalIndex, -1, dir)) return false;
    if (globalIndex + 2 * dir < 0 || globalIndex + 2 * dir >= 380 || Board::isOutOfBounds(globalIndex, 2, dir)) return false;
    const std::array<uint64_t, 6>& allyBitBoard = isBlack ? board.getBitBoardBlack() : board.getBitBoardWhite();
    std::array<uint64_t, 6>& enemyBitBoard = isBlack ? board.getBitBoardWhite() : board.getBitBoardBlack();

    const int firstEnemyGlobalIndex = (globalIndex - 1 * dir);
    const int secondEnemyGlobalIndex = (globalIndex + 2 * dir);
    const int allyGlobalIndex = (globalIndex + 1 * dir);
    const int firstEnemyArrayIndex = firstEnemyGlobalIndex / 64;
    const int firstEnemyIndex = firstEnemyGlobalIndex % 64;
    const int secondEnemyArrayIndex = secondEnemyGlobalIndex / 64;
    const int secondEnemyIndex = secondEnemyGlobalIndex % 64;
    const int allyArrayIndex = allyGlobalIndex / 64;
    const int allyIndex = allyGlobalIndex % 64;
    const uint64_t firstEnemyMask = 1ULL << firstEnemyIndex;
    const uint64_t secondEnemyMask = 1ULL << secondEnemyIndex;
    const uint64_t allyMask = 1ULL << allyIndex;


    if (enemyBitBoard[firstEnemyArrayIndex] & firstEnemyMask && enemyBitBoard[secondEnemyArrayIndex] &
        secondEnemyMask && allyBitBoard[allyArrayIndex] & allyMask) {
        return true;
    }
    return false;
}

bool CheckLegalMove::createsDoubleFreeThree(int posIndex, Board& board, const bool& isBlack) {
    int captures[8] = {-1};
    int count = 0;
    if (CaptureService::checkCapture(board, posIndex, isBlack, captures, count, false) != 0 ) {
        return false;
    }
    int freeThreeCount = 0;
    const std::array directions = {1, Board::SIZE + 1, Board::SIZE, Board::SIZE + 2};

    for (const int dir : directions) {
        const int patternIndex = board.getPatternIndex(posIndex, isBlack, dir);

        // Simulate ally stone in the middle of the pattern index
        const int testIndex = (patternIndex & ~(0b11 << 10)) | (0b01 << 10);
        constexpr int MASK_12 = 0b111111111111;

        // Create all slices of 12 bits (6 cases) to check for "Free three" patterns
        const int slice8 = (testIndex >> 8) & MASK_12;
        const int slice6 = (testIndex >> 6) & MASK_12;
        const int slice4 = (testIndex >> 4) & MASK_12;
        const int slice2 = (testIndex >> 2) & MASK_12;

        // The only formats possible for a "Free three" (O = Empty/0b00, A = Ally/0b01)
        constexpr int PATTERN_CONTIGUOUS_1 = 0b000001010100; // O O A A A O
        constexpr int PATTERN_CONTIGUOUS_2 = 0b000101010000; // O A A A O O
        constexpr int PATTERN_SPLIT_1 = 0b000100010100; // O A O A A O
        constexpr int PATTERN_SPLIT_2 = 0b000101000100; // O A A O A O

        bool isFreeThree = false;

        if (slice8 == PATTERN_CONTIGUOUS_1 || slice8 == PATTERN_SPLIT_1 || slice8 == PATTERN_SPLIT_2) {
            isFreeThree = true;
        }
        else if (slice6 == PATTERN_CONTIGUOUS_1 || slice6 == PATTERN_CONTIGUOUS_2 || slice6 == PATTERN_SPLIT_1) {
            isFreeThree = true;
        }
        else if (slice4 == PATTERN_CONTIGUOUS_1 || slice4 == PATTERN_CONTIGUOUS_2 || slice4 == PATTERN_SPLIT_2) {
            isFreeThree = true;
        }
        else if (slice2 == PATTERN_CONTIGUOUS_2 || slice2 == PATTERN_SPLIT_1 || slice2 == PATTERN_SPLIT_2) {
            isFreeThree = true;
        }

        if (isFreeThree) {
            freeThreeCount++;
            if (freeThreeCount >= 2) {
                return true;
            }
        }
    }

    return false;
}





#include "HeuristicService.h"
#include "AlignmentChecker.hpp"
#include "scores.hpp"


int HeuristicService::getHeuristicValue(const std::array<uint64_t, 4> left, const std::array<uint64_t, 4> &right) {
    Alignment alignmentRight = AlignmentChecker::checkAlignment(right);
    Alignment alignmentLeft = AlignmentChecker::checkAlignment(left);
    int nbStone = alignmentLeft.nbStone + alignmentRight.nbStone;

    if (alignmentLeft.blockDistance + alignmentRight.blockDistance < 5) {
        return 0;
    }

    if (nbStone == 4) {
        if (!alignmentLeft.hasHole && !alignmentRight.hasHole) {
            return Scores::OPEN_FOUR;
        }
        if (alignmentLeft.hasHole && alignmentRight.hasHole) {
            return Scores::FOUR;
        }
            return Scores::FOUR;
    }

    return 0;
}

std::array<uint64_t, 262144> HeuristicService::getHeuristicValues() {
    std::array<uint64_t, 262144> result;

    for (int i = 0; i < 262144; i++) {
        //TTTTSTTTT
    }
}


#ifndef ILLEGAL_MOVES_HPP
#define ILLEGAL_MOVES_HPP

struct IllegalMoves {
    enum Type {
        NONE,
        NOT_IN_BOARD,
        OCCUPIED,
        CREATE_CAPTURE,
        DOUBLE_FREE_CAPTURE,
        PRO_OPENING_FIRST_IN_MIDDLE,
        PRO_OPENING_THIRD_AT_DISTANCE
    };

    static constexpr const char* toString(const Type& move) {
        switch (move) {
            case NONE: return "[ERROR]";
            case NOT_IN_BOARD: return "the stone is not in board";
            case OCCUPIED: return "this cell is already occupied";
            case CREATE_CAPTURE: return "it creates a auto-capture";
            case DOUBLE_FREE_CAPTURE: return "it creates a double \"free-three\"";
            case PRO_OPENING_FIRST_IN_MIDDLE: return "it has to be in the\ncenter of the board";
            case PRO_OPENING_THIRD_AT_DISTANCE: return
                    "the third stone must be placed\nat least three intersections away from the first stone";
            default: return "[ERROR]";
        }
    }
};

#endif //ILLEGAL_MOVES_HPP

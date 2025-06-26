#ifndef ILLEGAL_MOVES_HPP
#define ILLEGAL_MOVES_HPP

struct IllegalMoves {
    enum Type {
        NONE,
        NOT_IN_BOARD,
        OCCUPIED,
        CREATE_CAPTURE,
        DOUBLE_FREE_CAPTURE
    };

    static constexpr const char* toString(const Type& move) {
        switch (move) {
            case NONE: return "[ERROR]";
            case NOT_IN_BOARD: return "the stone is not in board";
            case OCCUPIED: return "this cell is already occupied";
            case CREATE_CAPTURE: return "it creates a auto-capture";
            case DOUBLE_FREE_CAPTURE: return "it creates a double \"free-three\"";
            default: return "[ERROR]";
        }
    }
};

#endif //ILLEGAL_MOVES_HPP

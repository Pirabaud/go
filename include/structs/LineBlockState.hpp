#ifndef LINEBLOCKSTATE_HPP
#define LINEBLOCKSTATE_HPP

enum BlockState {
        NOTALIGN,
        FREE,
        SEMIBLOCKED,
        BLOCKED
};

struct Alignment {
        BlockState block;
        int nbAlignment;
};

#endif //LINEBLOCKSTATE_HPP

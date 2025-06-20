//
// Created by pirabaud on 6/19/25.
//

#include "../../include/CheckMovesService.h"


class CheckMovesService {

public:
    static enum IllegalMoves {
        NONE = "[SANS RAISON]",
        CREATE_CAPTURE = "cela crée une capture",
        DOUBLE_FREE_CAPTURE = "cela crée un double \"free-three\""
    };

    //static bool isIllegalMove(const std::string& illegalMove);

private:
    CheckMovesService();
};
//
// Created by Pierre Rabaud on 06/10/2025.
//

#ifndef GOMOKU_AISERVICE_H
#define GOMOKU_AISERVICE_H
#include "AiPlay.hpp"
#include "Board.h"


class AIService {
public:
    static AiPlay AIPlay(Board board);
};


#endif //GOMOKU_AISERVICE_H
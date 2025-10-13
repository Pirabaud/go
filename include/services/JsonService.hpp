//
// Created by Pierre Rabaud on 13/10/2025.
//

#ifndef GOMOKU_JSONSERVICE_HPP
#define GOMOKU_JSONSERVICE_HPP
#include <string>
#include <nlohmann/json.hpp>
using nlohmann::json;


#include "Position.hpp"


class JsonService {
public:
    struct IdGen {
        unsigned long long next = 1;
        std::string make() { return std::to_string(next++); }
    };

    static std::string toMoveStr(const Position& p);

    static void pushNode(json& tree,
                         const std::string& id,
                         const Position& move,
                         int depth,
                         const std::optional<std::string>& parent,
                         const std::optional<int>& heuristic);

};


#endif //GOMOKU_JSONSERVICE_HPP
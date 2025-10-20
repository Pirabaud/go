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
        static std::string make(const Position& pos, int depth);

    static std::string toMoveStr(const Position& p);

    static void pushNode(json& tree,
                         const std::string& id,
                         const Position& move,
                         int depth,
                         const std::optional<std::string>& parent,
                         const std::optional<int>& heuristic);

    static void setHeuristic(json &tree, const std::string &id, int value);
};


#endif //GOMOKU_JSONSERVICE_HPP
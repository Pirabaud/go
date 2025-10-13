//
// Created by Pierre Rabaud on 13/10/2025.
//

#include "../../include/services/JsonService.hpp"

std::string JsonService::toMoveStr(const Position &p) {
    return std::to_string(p.x) + "," + std::to_string(p.y);
}

void JsonService::pushNode(json &tree, const std::string &id, const Position &move, int depth,
                           const std::optional<std::string> &parent, const std::optional<int> &heuristic) {
    json node;
    node["id"] = id;
    node["move"] = toMoveStr(move);
    node["depth"] = depth;
    node["parent"] = parent.has_value() ? json(*parent) : json(nullptr);
    node["heuristic"] = heuristic.has_value() ? json(*heuristic) : json(nullptr);
    tree.push_back(std::move(node));
}

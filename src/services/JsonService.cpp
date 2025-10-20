//
// Created by Pierre Rabaud on 13/10/2025.
//

#include "../../include/services/JsonService.hpp"

std::string JsonService::make(const Position &pos, const int depth) {
    return std::to_string(pos.x) + std::to_string(pos.y) + std::to_string(depth);
}

std::string JsonService::toMoveStr(const Position &p) {
    return std::to_string(p.x) + "," + std::to_string(p.y);
}

void JsonService::pushNode(json &tree, const std::string &id, const Position &move, int depth,
                           const std::optional<std::string> &parent, const std::optional<int> &heuristic) {
    json node;
    node["id"] = id;
    node["move"] = std::to_string(move.x) + "," + std::to_string(move.y);;
    node["depth"] = depth;
    node["parent"] = parent.has_value() ? json(*parent) : json(nullptr);
    node["heuristic"] = heuristic.has_value() ? json(*heuristic) : json(nullptr);
    tree.push_back(std::move(node));
}

void JsonService::setHeuristic(json& tree, const std::string& id, int value) {
    for (auto& node : tree) {
        if (node.contains("id") && node["id"] == id) {
            node["heuristic"] = value;
            return;
        }
    }
}


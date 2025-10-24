// JsonService.hpp
#ifndef GOMOKU_JSONSERVICE_HPP
#define GOMOKU_JSONSERVICE_HPP

#include <string>
#include <nlohmann/json.hpp>
using nlohmann::json;

#include "Position.hpp"

class JsonService {
public:
    static void pushNode(json& tree,
                         int heuristic,
                         int depth,
                         int alpha,
                         int beta,
                         const Position& move,
                         const json& children = json::array());

    static void addChildToNode(json& tree,
                              const std::string& parentId,
                              const json& childNode);

    static std::string generateNodeId(const Position& move, int depth);

private:
    static int nodeCounter;
};

#endif // GOMOKU_JSONSERVICE_HPP
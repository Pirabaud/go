// JsonService.cpp
#include "../../include/services/JsonService.hpp"
#include <sstream>

int JsonService::nodeCounter = 0;

void JsonService::pushNode(json& tree,
                           int heuristic,
                           int depth,
                           int alpha,
                           int beta,
                           const Position& move,
                           const json& children) {
    
    json node = json::array();
    node.push_back(heuristic);      // Index 0: heuristic
    node.push_back(depth);          // Index 1: depth
    node.push_back(alpha);          // Index 2: alpha
    node.push_back(beta);           // Index 3: beta
    
    // Index 4: move as [x, y]
    json moveArray = json::array();
    moveArray.push_back(move.x);
    moveArray.push_back(move.y);
    node.push_back(moveArray);
    
    // Index 5: children array
    node.push_back(children);
    
    tree.push_back(node);
}

std::string JsonService::generateNodeId(const Position& move, int depth) {
    std::stringstream ss;
    ss << move.x << "_" << move.y << "_" << depth << "_" << nodeCounter++;
    return ss.str();
}
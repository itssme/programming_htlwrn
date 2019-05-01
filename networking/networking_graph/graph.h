#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <node.h>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iostream>

class Graph {
public:
    Graph();
    std::vector<Node> get_route(std::string from, std::string to);
private:
    std::vector<Node*> nodes;
    Node* get_node_from_name(std::string name);
};

#endif // GRAPH_H

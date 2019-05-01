#include "graph.h"

Graph::Graph() {
    Node *eisenstadt = new Node("Eisenstadt", 10);
    Node *klagenfurt = new Node("Klagenfurt", 11);
    Node *sankt = new Node("Sankt Pölten", 5);
    Node *linz = new Node("Linz", 5);
    Node *salzburg = new Node("Salzburg", 4);
    Node *graz = new Node("Graz", 3);
    Node *innsbruck = new Node("Innsbruck", 3);
    Node *bregenz = new Node("Bregenz", 8);
    Node *wien = new Node("Wien", 1);

    nodes.push_back(eisenstadt);
    nodes.push_back(klagenfurt);
    nodes.push_back(sankt);
    nodes.push_back(linz);
    nodes.push_back(salzburg);
    nodes.push_back(graz);
    nodes.push_back(innsbruck);
    nodes.push_back(bregenz);
    nodes.push_back(wien);

    eisenstadt->add_neighbour(sankt, 67);
    eisenstadt->add_neighbour(graz, 106);
    eisenstadt->add_neighbour(wien, 47);

    klagenfurt->add_neighbour(graz, 144);
    klagenfurt->add_neighbour(innsbruck, 240);
    klagenfurt->add_neighbour(salzburg, 144);

    sankt->add_neighbour(wien, 61);
    sankt->add_neighbour(linz, 74);
    sankt->add_neighbour(graz, 161);
    sankt->add_neighbour(eisenstadt, 67);

    linz->add_neighbour(sankt, 74);
    linz->add_neighbour(graz, 147);
    linz->add_neighbour(salzburg, 77);

    salzburg->add_neighbour(linz, 77);
    salzburg->add_neighbour(graz, 180);
    salzburg->add_neighbour(klagenfurt, 144);
    salzburg->add_neighbour(innsbruck, 132);

    graz->add_neighbour(eisenstadt, 106);
    graz->add_neighbour(sankt, 161);
    graz->add_neighbour(linz, 147);
    graz->add_neighbour(salzburg, 180);
    graz->add_neighbour(klagenfurt, 144);

    innsbruck->add_neighbour(salzburg, 132);
    innsbruck->add_neighbour(bregenz, 130);

    bregenz->add_neighbour(innsbruck, 130);

    wien->add_neighbour(eisenstadt, 47);
    wien->add_neighbour(sankt, 61);
}

std::vector<Node> Graph::get_route(std::string from_name, std::string to_name) {
    std::vector<Node> route;

    std::unordered_map<Node*, Node*> came_from;
    std::unordered_map<Node*, int> cost_so_far;
    std::priority_queue<std::pair<int, Node*>> frontier;

    Node* from = this->get_node_from_name(from_name);
    Node* to = this->get_node_from_name(to_name);

    frontier.push(std::make_pair(0, from));
    came_from[from] = from;
    cost_so_far[from] = 0;

    bool done = false;
    while (frontier.size() != 0 && !done) {
        Node* current = frontier.top().second;
        frontier.pop();

        for (auto node_value: current->connected) {
            Node* node = node_value.first;
            int new_cost = cost_so_far[current] + node->value + node_value.second;
            if (node->name == to_name) {
                came_from[node] = current;
                std::cout << "found end with " << new_cost << " from " << current->name << std::endl;
                done = true;
            } else if (cost_so_far.find(node) == cost_so_far.end() || cost_so_far[node] > new_cost) {
                std::cout << "adding " + node->name + " with " << -1*new_cost << " from " << current->name << " against old " << cost_so_far[node] << std::endl;
                came_from[node] = current;
                cost_so_far[node] = new_cost;
                frontier.push(std::make_pair(-1*new_cost, node));
            }
        }
    }

    if (!done) {
        return route;
    }

    Node* node = to;
    while (node->name != from_name) {
        route.push_back(Node(*node));
        node = came_from[node];
    }
    route.push_back(Node(*node));

    std::reverse(route.begin(), route.end());

    return route;
}

Node *Graph::get_node_from_name(std::string name) {
    for (unsigned long int i = 0; i < this->nodes.size(); i++) {
        if (this->nodes.at(i)->name == name) {
            return this->nodes.at(i);
        }
    }
}

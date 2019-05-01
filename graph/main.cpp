#include <QCoreApplication>

#include <graph.h>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    std::cout << "[!] started graph application" << std::endl;

    std::cout << "[!] creating graph" << std::endl;
    Graph graph = Graph();
    std::cout << "[!] done" << std::endl;

    while (true) {
        std::cout << "[!] FROM:" << std::endl;
        std::string from;
        std::cin >> from;
        std::cout << "[!] TO:" << std::endl;
        std::string to;
        std::cin >> to;

        std::vector<Node> route = graph.get_route(from, to);
        std::cout << "[!] ROUTE:" << std::endl;
        for (unsigned long int i = 0; i < route.size(); i++) {
            std::cout << "" << i << ": " << route.at(i).name << std::endl;
        }
    }

    return a.exec();
}

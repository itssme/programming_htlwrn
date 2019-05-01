#include <QCoreApplication>

#include <graph.h>
#include <iostream>
#include "networking_server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    std::cout << "[!] started graph application" << std::endl;
    Server* server = new Server();

    return a.exec();
}

#include <QCoreApplication>
#include <iostream>

#include "gramma.h"

// test input:
// 1 + 1
// 2 * 2
// 2 * ( 1 + 10 )
// 5 * ( 3 ^ 2 + 2 )
// 5 / 10 * 2
// 10 + -2
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    while (true) {
        std::string input;
        std::getline(std::cin, input);
        qDebug() << "[!] got input -> " << QString(input.c_str());
        QStringList token_list = QString(input.c_str()).split(" ");

        std::vector<std::string> tokens;
        tokens.reserve(static_cast<unsigned long int>(token_list.size() + 1));
        for (auto token: token_list) {
            tokens.push_back(token.toStdString());
        }
        tokens.push_back("stop");

        for (auto token: tokens) {
            qDebug() << "token: '" << QString(token.c_str()) << "'";
        }

        Parser parser{tokens};
        qDebug() << parser.parse();
    }

    return a.exec();
}

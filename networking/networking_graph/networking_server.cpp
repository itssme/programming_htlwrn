#include "networking_server.h"

void Connection::on_data_received() {
    emit route_from_client_received(socket->readAll());
}

void Connection::route_reply_receiver(QString repl) {
    socket->write(repl.toStdString().data());
    socket->flush();
}

Connection::Connection(QTcpSocket *_sock) : socket{_sock} {
    connect(socket, &QTcpSocket::readyRead, this, &Connection::on_data_received);
}

void Connection::run() {
    this->exec();
}

Server::Server() : graph{Graph()} {
    this->server = new QTcpServer();
    this->server->listen(QHostAddress::Any, 6969);
    connect(this->server, &QTcpServer::newConnection, this, &Server::on_new_connection);
}

void Server::on_new_connection() {
    qDebug() << "[!] got new connection";
    QTcpSocket* _sock = server->nextPendingConnection();
    Connection* ss = new Connection{_sock};
    connect(ss, &Connection::route_from_client_received, this, &Server::on_route_received);
    connect(this, &Server::route_reply, ss, &Connection::route_reply_receiver);
    ss->start();
}

void Server::on_route_received(QString route) {
    qDebug() << "[!] got a route request from client -> " << route;
    try {
        std::vector<std::string> cities;
        QXmlStreamReader* reader = new QXmlStreamReader(route);
        while (reader->readNextStartElement()) {
            if (reader->name() == "cities") {
                while (reader->readNextStartElement()) {
                    if (reader->name() == "city") {
                        cities.push_back(reader->readElementText().toStdString());
                    } else {
                        reader->skipCurrentElement();
                    }
                }
            } else {
                // invalid xml
                qDebug() << "[!] invalid xml from client -> " + route;
                emit route_reply(QString("invalid input"));
                return;
            }
        }

        std::vector<std::string> calculated_route_vec;
        QString calculated_route;
        for (unsigned long int i = 0; i < cities.size() - 1; i++) {
            std::vector<Node> tmp_route = this->graph.get_route(cities.at(i), cities.at(i+1));
            for (unsigned long int j = 0; j < tmp_route.size(); j++) {
                if (i == 0 || j != 0) {
                    calculated_route_vec.push_back(tmp_route.at(j).name);
                }
            }
        }

        QString to_send;
        QString send_route = calculated_route;
        QXmlStreamWriter* xml_writer = new QXmlStreamWriter(&to_send);
        xml_writer->writeStartDocument();
        xml_writer->writeStartElement("cities");
        for (auto city: calculated_route_vec) {
            xml_writer->writeTextElement("city", city.c_str());
        }
        xml_writer->writeEndElement();
        xml_writer->writeEndDocument();

        emit route_reply(to_send);
    } catch (...) {
        qDebug() << "[!] error during route calucaltion";
        emit route_reply(QString("invalid input"));
    }
}

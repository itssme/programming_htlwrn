#include "networking_client.h"

NetworkingClient::NetworkingClient() {
    sock = new QTcpSocket();
    sock->connectToHost("127.0.0.1", 2000);
    if (sock->waitForConnected()) {
        qDebug() << "Successfully connected!";
        connect(sock, &QTcpSocket::readyRead, this, &NetworkingClient::on_data_recived);

    } else {
        qDebug() << "Failed!";
    }

    sock->write("request");
    sock->flush();
}

void NetworkingClient::on_data_recived() {

}

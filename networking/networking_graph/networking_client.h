#ifndef NETWORKING_CLIENT_H
#define NETWORKING_CLIENT_H

#include <QTcpSocket>

class NetworkingClient {
public:
    NetworkingClient();
private slots:
    void on_data_recived();
private:
    QTcpSocket* sock;
};

#endif // NETWORKING_CLIENT_H

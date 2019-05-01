#ifndef NETWORKING_SERVER_H
#define NETWORKING_SERVER_H

#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QXmlStreamReader>
#include <graph.h>

class Server : public QObject {
    Q_OBJECT
public:
    Server();
private:
    Graph graph;
    QTcpServer* server;
private slots:
    void on_new_connection();
public slots:
    void on_route_received(QString route);
signals:
    void route_reply(QString reply);
};

class Connection : public QThread {
    Q_OBJECT
private:
    QTcpSocket* socket;
private slots:
    void on_data_received();
public slots:
    void route_reply_receiver(QString repl);
signals:
    void route_from_client_received(QString sql);
public:
    Connection(QTcpSocket* sock);
protected:
    virtual void run() override;
};

#endif // NETWORKING_SERVER_H

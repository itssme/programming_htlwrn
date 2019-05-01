#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_sendBtn_clicked() {
    if (connected) {
        this->sock->close();

        connected = false;
    }

    if (!connected) {
        sock = new QTcpSocket();
        sock->connectToHost(ui->hostInput->text(), quint16(ui->portInput->text().toInt()));
        if (sock->waitForConnected()) {
            qDebug() << "[!] successfully connected";
            connected = true;
            connect(sock, &QTcpSocket::readyRead, this, &MainWindow::on_data_received);
        } else {
            qDebug() << "[!] connection failed";
        }
    }

    QString to_send;
    QString route = ui->routeInput->text();
    QStringList route_listed = route.split(";");
    QXmlStreamWriter* xml_writer = new QXmlStreamWriter(&to_send);
    xml_writer->writeStartDocument();
    xml_writer->writeStartElement("cities");
    for (auto city: route_listed) {
        xml_writer->writeTextElement("city", city);
    }
    xml_writer->writeEndElement();
    xml_writer->writeEndDocument();

    sock->write(to_send.toStdString().data());
    sock->flush();
}

void MainWindow::on_data_received() {
    QString reply = sock->readAll().toStdString().data();
    qDebug() << reply;

    if (reply == "invalid input") {
        ui->messages->append(reply);
        return;
    }

    std::vector<std::string> cities;
    QXmlStreamReader* reader = new QXmlStreamReader(reply);
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
            reader->skipCurrentElement();  // probably invalid xml
            return;
        }
    }

    if (cities.size() == 0) {
        ui->messages->append("invalid input");
        return;
    }

    QString routes;
    for (unsigned long int i = 0; i < cities.size() - 1; i++) {
        routes += QString(cities.at(i).c_str()) + " -> ";
    }
    routes += QString(cities.at(cities.size()-1).c_str());

    ui->messages->append(routes);
}

#include <QApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QFile>

class TcpServer : public QTcpServer
{
public:
    TcpServer(QObject *parent = nullptr) : QTcpServer(parent) {
        connect(this, &TcpServer::newConnection, this, &TcpServer::on_new_connection);
    }

    void on_new_connection() {
        QTcpSocket *socket = nextPendingConnection();             // We get a new client socket
        QFile file("response.txt");                               // Open the response.txt file
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray file_data = file.readAll();
            QDataStream stream(socket);                           // Send the file to the client
            stream.writeBytes(file_data.data(), file_data.size());
            socket->disconnectFromHost();                         // Close the connection
            socket->waitForDisconnected();
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TcpServer server;
    if (!server.listen(QHostAddress::Any, 12345)) {
        qDebug() << "Could not start server";
    } else {
        qDebug() << "Server started on port" << server.serverPort();
    }
    return app.exec();
}


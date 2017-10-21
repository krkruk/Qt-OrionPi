#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <QScopedPointer>
#include <QDataStream>
#include <QHostAddress>
#include "include/IfceServer.h"

class QUdpSocket;


/**
 * @brief The UdpServer class is a simple wrapper that provides a UDP connection
 * capabilities.
 *
 * The UdpServer provides 1-1 connection with a remote UdpSocket. The server
 * stores most recent socket connetion data (IP address and port) and any
 * response message done via send() uses that data.
 */
class UdpServer : public IfceServer
{
    Q_OBJECT
public:
    /**
     * @brief UdpServer Creates an object of the UdpServer and starts
     * listening at given address and port.
     * @param bindAddress
     * @param bindPort
     * @param parent
     */
    UdpServer(const QHostAddress &bindAddress, int bindPort, QObject *parent=nullptr);
    ~UdpServer();

    /**
     * @brief listen Listens for any data at given connection data.
     *
     * The method binds to the address and the port and allows the server
     * to await for incoming data.
     * @param address
     * @param port
     */
    void listen(const QHostAddress &address, int port) override;
    void send(const QString &data) override;
    void send(const std::string &data) override;
    void send(const QByteArray &data) override;
    void setMaxPendingConnections(int numConnections) override;
    std::string lastReceived() const override;


private slots:
    void _on_ready_read_received();


private:
    void connections();
    void setup_conn_with_remote_socket();
    void write_to_server(const QByteArray &data);

    QScopedPointer<QUdpSocket> server;
    QHostAddress bindAddress;
    int bindPort {0};

    QDataStream in;
    std::string lastDatagram;
    QHostAddress clientAddress {QHostAddress::Any};
    int clientPort {0};
};

#endif // UDPSERVER_H

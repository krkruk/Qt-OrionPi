#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#include <QScopedPointer>
#include <QHostAddress>
#include <QDataStream>
#include "include/IfceIpSocket.h"
class QUdpSocket;

/**
 * @brief The UdpSocket class allows 1-1 connection with UdpServer.
 *
 * The class is a simple wrapper around QUdpSocket so it mimics
 * client-server connection with connectionless protocol.
 */
class UdpSocket : public IfceIpSocket
{
    Q_OBJECT
public:
    /**
     * @brief UdpSocket Constructs a socket.
     * @param bindAddress Binding address - the address this socket is to be bound to.
     * @param bindPort Port address - the port this socket is to be bound to.
     * 0 value is possible, however not advised. Due to connectionless nature of
     * UDP server bind port must be set explicitly (cannot change during the span
     * of the virtual udp connection);
     * @param parent
     */
    UdpSocket(const QHostAddress &bindAddress, int bindPort, QObject *parent=nullptr);
    ~UdpSocket();

    /**
     * @brief connectToHost Connects to the server.
     * The method also sends a message that allows the server to determine an address
     * and a port of a remote client (this object).  If any other UdpSocket connects
     * to the UdpServer, connectToHost() must be called once more.
     * @param address Address of the server
     * @param port Port of the server
     */
    void connectToHost(const QHostAddress &address, int port) override;
    void disconnectFromHost() override;
    void send(const QString &data) override;
    void send(const std::string &data) override;
    void send(const QByteArray &data) override;
    std::string lastServerResponse() const override;
    bool isOpen() const override;

private slots:
    void _on_ready_read_received();


private:
    void connections();
    void write_to_server(const QByteArray &data);

    QScopedPointer<QUdpSocket> client;
    QHostAddress bindAddress;
    int bindPort {0};

    QDataStream in;
    std::string lastDatagram;
};

#endif // UDPSOCKET_H

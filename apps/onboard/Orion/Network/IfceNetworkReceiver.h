#ifndef IFCENETWORKRECEIVER_H
#define IFCENETWORKRECEIVER_H

class QByteArray;


class IfceNetworkReceiver
{
public:
    virtual ~IfceNetworkReceiver() {}

    virtual void onRemoteDataReceived(const QByteArray &data) = 0;
};

#endif // IFCENETWORKRECEIVER_H

#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H
#include <QByteArray>
#include <QHash>
#include <QSharedPointer>

#include "interface/IfceSerialModel.h"
#include "interface/IfceSerialController.h"

class SerialController : public IfceSerialController
{
public:
    SerialController();
    ~SerialController();

    void addModel(QSharedPointer<IfceSerialModel> model) override;
    void onLineReceived(int id, const QByteArray &line) override;
    QByteArray getLastReceived(int id) const override;

private:
    QHash<int, QByteArray> lastData;
    QHash<int, QSharedPointer<IfceSerialModel>> models;
};

#endif // SERIALCONTROLLER_H

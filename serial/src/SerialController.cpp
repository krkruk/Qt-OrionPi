#include "SerialController.h"

SerialController::SerialController()
{

}

SerialController::~SerialController()
{

}

void SerialController::addModel(QSharedPointer<IfceSerialModel> model)
{
    const auto id { model->getId() };
    models[id] = model;
}

void SerialController::onLineReceived(int id, const QByteArray &line)
{
    lastData[id] = line;
    if( !models.value(id).isNull() )
        models[id]->updateModel(line);
}

QByteArray SerialController::getLastReceived(int id) const
{
    return lastData.value(id, 0);
}

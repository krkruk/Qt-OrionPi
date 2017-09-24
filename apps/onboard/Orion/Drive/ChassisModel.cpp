#include "ChassisModel.h"
#include "IfceWheelModel.h"
#include "IfceChassisFeedbackGenerator.h"
#include <QDebug>

using namespace Orion;


ChassisModel::ChassisModel(QObject *parent)
    : QObject(parent)
{
}

ChassisModel::~ChassisModel()
{
}

void ChassisModel::setDriveAlgorithm(QSharedPointer<IfceDriveMode> algorithm)
{
    driveAlgorithm = algorithm;
}

void ChassisModel::addWheel(QSharedPointer<IfceWheelModel> wheel)
{
    const auto id { wheel->getId() };
    if( IfceDriveMode::isLeftRow(id) )
        leftRowWheels[id] = wheel;
    else
        rightRowWheels[id] = wheel;
}

void ChassisModel::notifyAll()
{
    notify_left_side();
    notify_right_side();
}

void ChassisModel::updateState(const QByteArray &cmd)
{
    if( driveAlgorithm.isNull() )
        return;

    driveAlgorithm->processInput(cmd);
    const auto leftSideValue { driveAlgorithm->getValue(Orion::IfceDriveMode::LEFT_0) };
    const auto rightSideValue { driveAlgorithm->getValue(Orion::IfceDriveMode::RIGHT_0) };

    update_angular_velocities_left_side(leftSideValue);
    update_angular_velocities_right_side(rightSideValue);
}

void ChassisModel::setFeedbackGeneratorAlgorithm(QSharedPointer<IfceChassisFeedbackGenerator> generator)
{
    feedbackGenerator = generator;
}

QByteArray ChassisModel::getFeedbackData()
{
    if( !feedbackGenerator )
        return QByteArray();
    feedbackGenerator->clear();

    for( const auto &leftWheel : leftRowWheels.values() )
        feedbackGenerator->addInput(leftWheel);

    for( auto &rightWheel : rightRowWheels.values() )
        feedbackGenerator->addInput(rightWheel);

    return feedbackGenerator->generate();
}

void ChassisModel::update_angular_velocities_left_side(double angVelLeftSide)
{
    for( auto &leftWheel : leftRowWheels.values() )
        if( leftWheel ) leftWheel->updateAngularVelocity(angVelLeftSide);
}

void ChassisModel::update_angular_velocities_right_side(double angVelRightSide)
{
    for( auto &rightWheel : rightRowWheels.values() )
        if( rightWheel ) rightWheel->updateAngularVelocity(angVelRightSide);
}

void ChassisModel::notify_left_side()
{
    for( auto &leftWheel : leftRowWheels.values() )
        if( leftWheel ) leftWheel->notifyObservers();
}

void ChassisModel::notify_right_side()
{
    for( auto &rightWheel : rightRowWheels.values() )
        if( rightWheel ) rightWheel->notifyObservers();
}


#include "DriveFeedbackWidget.h"
#include "ui_DriveFeedbackWidget.h"
#include <QDebug>


DriveFeedbackWidget::DriveFeedbackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriveFeedbackWidget)
{
    ui->setupUi(this);
}

DriveFeedbackWidget::~DriveFeedbackWidget()
{
    delete ui;
}

void DriveFeedbackWidget::setWheelName(const QString &name)
{
    ui->groupBoxWheel->setTitle(name);
}

void DriveFeedbackWidget::setAngularVelocity(double degPerSecond)
{
    ui->labelAngularVelocity->setText(QString::number(degPerSecond));
}

void DriveFeedbackWidget::setHeatSinkTemperature(double centigrades)
{
    ui->labelHeat->setText(QString::number(centigrades));
}

void DriveFeedbackWidget::setCurrent(double ampers)
{
    ui->labelCurrent->setText(QString::number(ampers));
}

void DriveFeedbackWidget::setPwm(int pwm)
{
    ui->labelPwm->setText(QString::number(pwm));
}

void DriveFeedbackWidget::setErrorCode(int error)
{
    ui->labelErrorCode->setText(QString::number(error));
}

void DriveFeedbackWidget::clear()
{
    ui->labelAngularVelocity->setText("0");
    ui->labelHeat->setText("0");
    ui->labelCurrent->setText("0");
    ui->labelPwm->setText("0");
    ui->labelErrorCode->setText("0");
}

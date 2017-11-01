#include "DriveFeedbackWidget.h"
#include "ui_DriveFeedbackWidget.h"
#include <QPalette>
#include <QDebug>


DriveFeedbackWidget::DriveFeedbackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriveFeedbackWidget)
{
    ui->setupUi(this);
    ui->frame->setAutoFillBackground(true);
    ui->frame->setFrameShape(QFrame::Box);
    ui->frame->setFrameShadow(QFrame::Plain);
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
    QColor bgColor = error == 0 ? Qt::transparent : Qt::red;
    QPalette palette = ui->frame->palette();
    palette.setColor(QPalette::Background, bgColor);
    ui->frame->setPalette(palette);

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

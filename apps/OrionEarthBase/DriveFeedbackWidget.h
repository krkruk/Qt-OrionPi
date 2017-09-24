#ifndef DRIVEFEEDBACKWIDGET_H
#define DRIVEFEEDBACKWIDGET_H

#include <QWidget>

namespace Ui {
class DriveFeedbackWidget;
}

class DriveFeedbackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DriveFeedbackWidget(QWidget *parent = 0);
    ~DriveFeedbackWidget();

    void setWheelName(const QString &name);
    void setAngularVelocity(double degPerSecond);
    void setHeatSinkTemperature(double centigrades);
    void setCurrent(double ampers);
    void setPwm(int pwm);
    void setErrorCode(int error);
    void clear();


private:
    Ui::DriveFeedbackWidget *ui;
};

#endif // DRIVEFEEDBACKWIDGET_H

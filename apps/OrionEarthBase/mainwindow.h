#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QScopedPointer>
#include <QPointer>
#include <QHash>
#include "DriveFeedbackWidget.h"
#include "include/IfceIpSocket.h"

namespace Ui {
class MainWindow;
}

class QGamepad;
class QTimer;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onRemoteMessageReceived(const QByteArray &message);
    void onGamepadIntervalTimeout();
    void onMaximumSliderChanged(int value);
    void on_pushButtonAcceptGamepad_clicked();
    void on_action_Refresh_triggered();

    void on_action_Settings_triggered();

private:
    void connections();
    void setup_gamepad_timer();
    void start_connection();
    void display_available_gamepads();
    void parse_feedback_data(const QByteArray &data);
    void apply_data_to_feedback_widget(const QString &name, double angularVelocity,
                                       double heatSinkTemp, double current,
                                       int pwm, int errorCode);

    double process_input(double input) const;
    double apply_dead_zone(double input) const;
    void apply_gamepad_to_widget();
    void write_gamepad_input_to_server();

    Ui::MainWindow *ui;
    QScopedPointer<IfceIpSocket, QScopedPointerDeleteLater> socket;
    QHash<QString, QPointer<DriveFeedbackWidget>> feedbackWidgets;
    QScopedPointer<QTimer> gamepadTimer;
    QScopedPointer<QGamepad, QScopedPointerDeleteLater> gamepad;

    int inputMultiplier { 1 };
    double leftAxis { 0.0 };
    double rightAxis { 0.0 };
};

#endif // MAINWINDOW_H

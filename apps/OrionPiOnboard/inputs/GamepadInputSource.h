#ifndef GAMEPADINPUTSOURCE_H
#define GAMEPADINPUTSOURCE_H
#include <QScopedPointer>
#include <QPointer>
#include "IfceInputSource.h"

class QTimer;
class QGamepad;


class GamepadInputSource : public IfceInputSource
{
    Q_OBJECT
public:
    explicit GamepadInputSource(const QString &organization,
                                const QString &application, QObject *parent = nullptr);
    ~GamepadInputSource();

    void send(const QByteArray &data) override;
    SOURCE_TYPE type() const override;


private slots:
    void onPingerTimeout();


private:
    void load_settings();
    void setup_gamepad_pinger();
    void create_gamepad();
    double process_input(double input) const;
    double apply_dead_zone(double input) const;
    void broadcast_gamepad_results();
    void store_settings();

    QString organization;
    QString application;

    QScopedPointer<QTimer> gamepadPinger;
    QPointer<QGamepad> gamepad;
    int pingInterval {50};
    int revertAxis {-1};
    int inputMultiplier { 100 };
    double deadZone {.10f};

    double leftAxis { 0.0 };
    double rightAxis { 0.0 };
};

#endif // GAMEPADINPUTSOURCE_H

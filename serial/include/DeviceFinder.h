#ifndef DEVICEFINDER_H
#define DEVICEFINDER_H
#include "IfceSerialFinder.h"

#include <QSerialPortInfo>
#include <QSerialPort>

/**
 * @brief The DeviceFinder class is responsible for finding serial devices.
 *
 * The class seeks for available serial devices. When discovered, it connects
 * to each other (one by one) and attempts to read the data. Received
 * data is then parsed with a {@link IfceDevice} that is created by
 * an instance of {@link IfceDeviceFactory}. The parsing step is of utmost
 * importance in the initial booting of the system. It allows assinging the serial
 * device to its model. The model then handles proper data parsing.
 */
class DeviceFinder : public IfceSerialFinder
{
    Q_OBJECT
public:
    explicit DeviceFinder(IfceDeviceFactory *factory, QObject *parent = nullptr);
    ~DeviceFinder();

    /**
     * @brief discover Discovers devices based on parsing data engine implemented in {@link IfceDevice}
     * @return List of found devices
     */
    QList<QSharedPointer<IfceDevice>> discover() override;


    void setSerialSettings(QSharedPointer<IfceSerialSettings> settings);


protected:
    /**
     * @brief run_serial Runs action of the serial
     * Runs action of the serial. It tries to identify the ID of the device
     * based on a single line received from it.
     * @param serialInfo Information about serial port the method connects to.
     * @return Instance of {@link SerialDeviceIfce}. If the method cannot create
     * an instance, returns an empty QSharedPointer
     */
    QSharedPointer<IfceDevice> run_serial(const QSerialPortInfo &serialInfo);


private:
    static const char READ_LINE_ERR_MSG[];
    QScopedPointer<IfceDeviceFactory> factory;
    QSharedPointer<IfceSerialSettings> settings;

    /**
     * @brief open_serial Opens serial.
     * Opens serial. Emits a {@link serialError()} if cannot establish a connection.
     * Returns appropriate bool values
     * @param serial Reference to the instance of QSerialPort
     * @return True if connection is established. False otherwise.
     */
    bool open_serial(QSharedPointer<QSerialPort> &serial);

    /**
     * @brief wait_for_connection Waits for connection
     * Waits for connection for {@link SerialSettings::getWaitForReadLineMs()} period of time
     * @param serial Reference to the instance of QSerialPort
     * @return True if new data is available. False if an error occured or timeout.
     */
    bool wait_for_connection(QSharedPointer<QSerialPort> &serial);

    /**
     * @brief wait_for_read_line Waits for opportunity to read a line.
     * The method is responsible for reading a line that is essential in
     * a protocol implementation. See system documentation for more details.
     * It waits for {@link SerialSettings::instance()->getWaitForReadLineMs()} period of time.
     * @param serial Reference to the instance of QSerialPort
     * @return True if reading of a line is successful. False otherwise.
     */
    bool wait_for_read_line(QSharedPointer<QSerialPort> &serial);

    /**
     * @brief handle_error_msg Helper method
     * @param serial Reference to the instance of QSerialPort
     * @param msg Error message
     * @return Empty QSharedPointer<SerialDeviceIfce>
     */
    QSharedPointer<IfceDevice> handle_error_msg(QSharedPointer<QSerialPort> &serial, const QString &msg);
};

#endif // DEVICEFINDER_H

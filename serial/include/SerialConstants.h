#ifndef SERIALCONSTANTS_H
#define SERIALCONSTANTS_H

namespace CONST {
    namespace SETTINGS {
        namespace SERIAL {
        static constexpr char GROUP_NAME [] = "SERIAL";
        static constexpr char END_LINE [] = "\r\n";

            namespace KEYS {
                static constexpr char ID [] = "DEVICE_ID_LABEL";
                static constexpr char WAIT_FOR_READLINE_MS[] = "WAIT_FOR_READLINE_MS";
                static constexpr char BAUDRATE[] = "BAUDRATE";
                static constexpr char DATA_BITS[] = "DATABITS";
                static constexpr char FLOW_CONTROL[] = "FLOWCONTROL";
                static constexpr char PARITY[] = "PARITY";
                static constexpr char STOP_BITS[] = "STOPBITS";
                static constexpr char ATTEMPTS_READ_LINE[] = "ATTEMPT_READ_LINE";
            }

            namespace DEFAULT {
                static constexpr char ID_KEY [] = "ID";
                static constexpr int WAIT_FOR_READLINE_MS = 1000;
                static constexpr int BAUDRATE = 115200;
                static constexpr int DATABITS = 8;                  //8 bits {@link QSerialPort::DataBits}
                static constexpr int FLOW_CONTROL = 0;              //no flow control {@link QSerialPort::FlowControl}
                static constexpr int PARITY = 0;                    //no parity {@link QSerialPort::Parity}
                static constexpr int STOP_BITS = 1;                 //one stop bit {@link QSerialPort::StopBits}
                static constexpr int ATTEMPTS_READ_LINE = 3;
            }
        }
    }
}
#endif // SERIALCONSTANTS_H

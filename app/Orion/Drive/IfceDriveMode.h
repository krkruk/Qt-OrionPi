#ifndef IFCEDRIVEMODE_H
#define IFCEDRIVEMODE_H

class QByteArray;


namespace Orion {

    class IfceDriveMode
    {
    public:
        enum WHEEL {
            LEFT_0, RIGHT_0,
            LEFT_1, RIGHT_1,
            LEFT_2, RIGHT_2,
            LEFT_3, RIGHT_3
        };

        virtual ~IfceDriveMode(){}

        virtual void processInput(const QByteArray &rawData) = 0;
        virtual double getValue(int wheel) = 0;

        static bool isLeftRow(int wheel) { return !(wheel%2); }
        static bool isRightRow(int wheel) { return (wheel%2); }
    };

}
#endif // IFCEDRIVEMODE_H

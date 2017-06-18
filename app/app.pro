QT += core serialport concurrent network
QT -= gui

CONFIG += c++11

TARGET = OrionPiApp
DESTDIR = ${ORION_BUILDPATH}

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Orion/Drive/WheelModel.cpp \
    Orion/Drive/ChassisModel.cpp \
    Orion/Drive/DriveModeDirect.cpp \
    OrionDriveSettings.cpp \
    OrionEngine.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    ../GlobalConstants.h \
    Orion/Drive/WheelModel.h \
    Orion/Drive/ChassisModel.h \
    Orion/Drive/IfceDriveMode.h \
    Orion/Drive/DriveModeDirect.h \
    OrionDriveSettings.h \
    Orion/Drive/DriveConstants.h \
    Orion/Drive/IfceChassisModel.h \
    Orion/Network/IfceNetworkReceiver.h \
    Orion/Drive/IfceWheelModel.h \
    OrionEngine.h

win32:CONFIG(release, debug|release): LIBS += -L${ORION_BUILDPATH_LIBS} -lserial
else:win32:CONFIG(debug, debug|release): LIBS += -L${ORION_BUILDPATH_LIBS} -lseriald
else:unix: LIBS += -L${ORION_BUILDPATH_LIBS} -lserial

INCLUDEPATH += $$PWD/../serial
DEPENDPATH += $$PWD/../serial

win32:CONFIG(release, debug|release): LIBS += -L${ORION_BUILDPATH_LIBS} -lnetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L${ORION_BUILDPATH_LIBS} -lnetwork
else:unix: LIBS += -L${ORION_BUILDPATH_LIBS} -lnetwork

INCLUDEPATH += $$PWD/../network
DEPENDPATH += $$PWD/../network

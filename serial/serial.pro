#-------------------------------------------------
#
# Project created by QtCreator 2017-04-29T11:42:03
#
#-------------------------------------------------

QT       -= gui serialport concurrent
QT += serialport

TARGET = serial
TEMPLATE = lib
DESTDIR = ${ORION_BUILDPATH_LIBS}

DEFINES += SERIAL_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/SerialManager.cpp \
    src/SerialController.cpp \
    src/JsonSerialParser.cpp \
    src/JsonDeviceFactory.cpp \
    src/JsonDevice.cpp \
    src/DeviceFinder.cpp \
    src/ThreadedDeviceFinder.cpp \
    src/AsyncDeviceFinder.cpp \
    src/SerialFactory.cpp


HEADERS +=\
    include/serial_global.h \
    exceptions/ParsingException.h \
    include/interface/IfceSerialController.h \
    include/interface/IfceSerialModel.h \
    include/interface/IfceSerialObserver.h \
    include/interface/IfceSerialParser.h \
    include/interface/IfceSerialFinder.h \
    include/interface/IfceSerialSettings.h \
    include/interface/IfceDeviceFactory.h \
    include/interface/IfceDevice.h \
    include/DefaultSerialSettings.h \
    include/DeviceFinder.h \
    include/ThreadedDeviceFinder.h \
    include/AsyncDeviceFinder.h \
    include/SerialFactory.h \
    include/SerialManager.h \
    include/SerialController.h \
    include/JsonSerialParser.h \
    include/JsonDevice.h \
    include/JsonDeviceFactory.h


INCLUDEPATH += \
    include/

unix {
    target.path = /usr/lib
    INSTALLS += target
}

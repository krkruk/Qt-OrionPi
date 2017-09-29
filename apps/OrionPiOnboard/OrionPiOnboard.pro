QT += core serialport concurrent network gamepad
QT -= gui

CONFIG += c++11

TARGET = OrionPiOnboard
DESTDIR = $$(ORION_BUILDPATH)

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    Orion/Drive/interface/IfceDriveMode.h \
    Orion/Drive/interface/IfceChassisModel.h \
    Orion/Drive/interface/IfceWheelModel.h \
    Orion/Drive/interface/IfceChassisFeedbackGenerator.h \
    Orion/Drive/WheelModel.h \
    Orion/Drive/ChassisModel.h \
    settings/DriveSettings.h \
    Orion/Drive/DriveConstants.h \
    OrionEngine.h \
    settings/SerialConstants.h \
    settings/SerialSettings.h \
    Orion/Drive/JsonDriveModeDirect.h \
    Orion/Drive/JsonChassisFeedbackGenerator.h \
    Orion/Drive/ProtobufDriveModeDirect.h \
    Orion/Drive/ProtobufChassisFeedbackGenerator.h \
    ../protos/earthBaseToRoverComm.pb.h \
    ../protos/roverToEarthBaseComm.pb.h \
    inputs/IfceInputSource.h \
    inputs/TcpInputSource.h \
    inputs/GamepadInputSource.h

SOURCES += main.cpp \
    Orion/Drive/WheelModel.cpp \
    Orion/Drive/ChassisModel.cpp \
    settings/DriveSettings.cpp \
    OrionEngine.cpp \
    settings/SerialSettings.cpp \
    Orion/Drive/JsonDriveModeDirect.cpp \
    Orion/Drive/JsonChassisFeedbackGenerator.cpp \
    Orion/Drive/ProtobufDriveModeDirect.cpp \
    Orion/Drive/ProtobufChassisFeedbackGenerator.cpp \
    ../protos/earthBaseToRoverComm.pb.cc \
    ../protos/roverToEarthBaseComm.pb.cc \
    inputs/TcpInputSource.cpp \
    inputs/GamepadInputSource.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    ../protos

unix:!macx: LIBS += -L$$(ORION_BUILDPATH_LIBS) -lserial
INCLUDEPATH += $$PWD/../../serial
DEPENDPATH += $$PWD/../../serial

unix:!macx: LIBS += -L$$(ORION_BUILDPATH_LIBS) -lnetwork
INCLUDEPATH += $$PWD/../../network
DEPENDPATH += $$PWD/../../network

#LIBS += -L$$(PROTOBUF)/lib -lprotobuf
LIBS += $$(PROTOBUF)/lib/libprotobuf.a
INCLUDEPATH += $$(PROTOBUF)/include
DEPENDPATH += $$(PROTOBUF)/include
QMAKE_CXXFLAGS += -isystem $$(PROTOBUF)/include
QMAKE_CXXFLAGS += -isystem ../protos

DISTFILES += \
    ../protos/earthBaseToRoverComm.proto \
    ../protos/roverToEarthBaseComm.proto

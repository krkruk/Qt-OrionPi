#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T20:41:39
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_testdrivemodel
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11
TEMPLATE = app
DESTDIR = $$(ORION_BUILDPATH_TEST)/tst
QMAKE_CFLAGS_ISYSTEM = -I
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


DEFINES += SRCDIR=\\\"$$PWD/\\\"


unix:!macx: LIBS += -L$$(ORION_BUILDPATH_LIBS) -lserial

INCLUDEPATH += $$PWD/../../serial
DEPENDPATH += $$PWD/../../serial

INCLUDEPATH += \
    ../../apps/OrionPiOnboard/Orion/Drive/ \
    ../../apps/OrionPiOnboard \
    ../../apps/protos/

DEPENDPATH += \
    ../../apps/OrionPiOnboardOrion/Drive/ \
    ../../apps/OrionPiOnboard

HEADERS += \
    ../../apps/OrionPiOnboard/Orion/Drive/WheelModel.h \
    ../../apps/OrionPiOnboard/Orion/Drive/ChassisModel.h \
    ../../apps/OrionPiOnboard/Orion/Drive/JsonDriveModeDirect.h \
    ../../apps/OrionPiOnboard/Orion/Drive/ProtobufDriveModeDirect.h \
    ../../apps/OrionPiOnboard/settings/DriveSettings.h \
    ../../apps/OrionPiOnboard/Orion/Drive/interface/IfceChassisFeedbackGenerator.h \
    ../../apps/OrionPiOnboard/Orion/Drive/JsonChassisFeedbackGenerator.h \
    ../../apps/OrionPiOnboard/Orion/Drive/ProtobufChassisFeedbackGenerator.cpp \
    MockWheelObserver.h \
    ../../apps/protos/earthBaseToRoverComm.pb.h \
    ../../apps/protos/roverToEarthBaseComm.pb.h \
    ../../apps/protos/protocolEnums.pb.h

SOURCES += tst_testdrivemodel.cpp \
    ../../apps/OrionPiOnboard/Orion/Drive/WheelModel.cpp \
    ../../apps/OrionPiOnboard/Orion/Drive/JsonDriveModeDirect.cpp \
    ../../apps/OrionPiOnboard/Orion/Drive/ProtobufDriveModeDirect.cpp \
    ../../apps/OrionPiOnboard/settings/DriveSettings.cpp \
    ../../apps/OrionPiOnboard/Orion/Drive/ChassisModel.cpp \
    ../../apps/OrionPiOnboard/Orion/Drive/JsonChassisFeedbackGenerator.cpp \
    ../../apps/OrionPiOnboard/Orion/Drive/ProtobufChassisFeedbackGenerator.cpp \
    ../../apps/protos/earthBaseToRoverComm.pb.cc \
    ../../apps/protos/roverToEarthBaseComm.pb.cc \
    ../../apps/protos/protocolEnums.pb.cc

IS_PROTOBUF_STATIC = $$(PROTOBUF_STATIC)
equals( IS_PROTOBUF_STATIC , true ) {
    LIBS += $$(PROTOBUF)/lib/libprotobuf.a
} else {
    LIBS += -L$$(PROTOBUF)/lib -lprotobuf
}

INCLUDEPATH += $$(PROTOBUF)/include
DEPENDPATH += $$(PROTOBUF)/include
 

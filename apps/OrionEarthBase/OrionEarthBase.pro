#-------------------------------------------------
#
# Project created by QtCreator 2017-09-24T13:43:08
#
#-------------------------------------------------

QT       += core gui network gamepad

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OrionEarthBase
DESTDIR = $$(ORION_BUILDPATH)
TEMPLATE = app
CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    DriveFeedbackWidget.cpp \
    ../protos/earthBaseToRoverComm.pb.cc

HEADERS  += mainwindow.h \
    DriveFeedbackWidget.h \
    ../protos/earthBaseToRoverComm.pb.h

FORMS    += mainwindow.ui \
    DriveFeedbackWidget.ui

INCLUDEPATH += \
    ../protos/

unix:!macx: LIBS += -L$$(ORION_BUILDPATH_LIBS) -lnetwork
INCLUDEPATH += $$PWD/../../network
DEPENDPATH += $$PWD/../../network

# loading statically due to fucking Ubuntu protobuf collisions...
# to be solved in the future or just use Arch Linux
#LIBS += -L$$(PROTOBUF)/lib -lprotobuf
LIBS += $$(PROTOBUF)/lib/libprotobuf.a
INCLUDEPATH += $$(PROTOBUF)/include
DEPENDPATH += $$(PROTOBUF)/include

DISTFILES += \
    ../protos/earthBaseToRoverComm.proto

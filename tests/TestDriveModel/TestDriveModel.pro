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
    ../../apps/onboard/Orion/Drive/ \
    ../../apps/onboard
DEPENDPATH += \
    ../../apps/onboardOrion/Drive/ \
    ../../apps/onboard

HEADERS += \
    ../../GlobalConstants.h \
    ../../apps/onboard/Orion/Drive/WheelModel.h \
    ../../apps/onboard/Orion/Drive/ChassisModel.h \
    ../../apps/onboard/Orion/Drive/DriveModeDirect.h \
    ../../apps/onboard/OrionDriveSettings.h \
    MockWheelObserver.h

SOURCES += tst_testdrivemodel.cpp \
    ../../apps/onboard/Orion/Drive/WheelModel.cpp \
    ../../apps/onboard/Orion/Drive/DriveModeDirect.cpp \
    ../../apps/onboard/OrionDriveSettings.cpp \
    ../../apps/onboard/Orion/Drive/ChassisModel.cpp

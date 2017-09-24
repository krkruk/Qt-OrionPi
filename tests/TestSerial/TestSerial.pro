#-------------------------------------------------
#
# Project created by QtCreator 2017-04-29T09:39:05
#
#-------------------------------------------------

QT       += testlib serialport

QT       -= gui

TARGET = tst_testserialtest
#DESTDIR = ${ORION_BUILDPATH}/tests

CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

DESTDIR = $$(ORION_BUILDPATH_TEST)/tst

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += tst_testserialtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


win32:CONFIG(release, debug|release): LIBS += -L${ORION_BUILDPATH_LIBS} -lserial
else:win32:CONFIG(debug, debug|release): LIBS += -L${ORION_BUILDPATH_LIBS} -lseriald
else:unix: LIBS += -L${ORION_BUILDPATH_LIBS} -lserial

INCLUDEPATH += $$PWD/../../serial
DEPENDPATH += $$PWD/../../serial

HEADERS += \
    MockSerialModel.h \
    MockDeviceFinder.h \
    MockFoundDevice.h

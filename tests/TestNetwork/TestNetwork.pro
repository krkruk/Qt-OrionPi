#-------------------------------------------------
#
# Project created by QtCreator 2017-05-20T01:36:08
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_testnetwork
CONFIG   += console
CONFIG   -= app_bundle
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


SOURCES += tst_testnetwork.cpp \
    proto/testMsg.pb.cc

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += \
    ../../app/Orion/Drive/ \
    ../../app/Orion/Network/ \
    ../../app/

DEPENDPATH += \
    ../../app/Orion/Drive/ \
    ../../app/Orion/Network/ \
    ../../app/

HEADERS += \
    ../../GlobalConstants.h \
    proto/testMsg.pb.h

win32:CONFIG(release, debug|release): LIBS += -L${ORION_BUILDPATH_LIBS} -lnetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L${ORION_BUILDPATH_LIBS} -lnetwork
else:unix: LIBS += -L${ORION_BUILDPATH_LIBS} -lnetwork

INCLUDEPATH += $$PWD/../../network
DEPENDPATH += $$PWD/../../network

LIBS += -L$$(PROTOBUF)/lib -lprotobuf
INCLUDEPATH += $$(PROTOBUF)/include
DEPENDPATH += $$(PROTOBUF)/include

DISTFILES += \
    proto/testMsg.proto

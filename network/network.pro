#-------------------------------------------------
#
# Project created by QtCreator 2017-05-27T11:04:58
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = network
TEMPLATE = lib
DESTDIR = ${ORION_BUILDPATH_LIBS}
DEFINES += NETWORK_LIBRARY
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


HEADERS +=  network_global.h \
    include/IfceServer.h \
    TcpServer.h \
    TcpSocket.h \
    include/IfceIpSocket.h \
    UdpServer.h \
    UdpSocket.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

SOURCES += \
    TcpServer.cpp \
    TcpSocket.cpp \
    UdpServer.cpp \
    UdpSocket.cpp

#-------------------------------------------------
#
# Project created by QtCreator 2014-09-15T22:24:42
#
#-------------------------------------------------

QT       -= core gui

TARGET = x3manager
TEMPLATE = lib

LIBS += libole32

INCLUDEPATH +=../STT_Main/stt_interface/interface

DEFINES += X3MANAGER_LIBRARY

SOURCES += \
    module.cpp \
    plugins.cpp \
    workpath.cpp

HEADERS += \
    plugins.h \
    workpath.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

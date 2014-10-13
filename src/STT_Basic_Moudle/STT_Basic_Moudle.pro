#-------------------------------------------------
#
# Project created by QtCreator 2014-09-16T21:24:21
#
#-------------------------------------------------

QT       -= core gui

TARGET = STT_Basic_Moudle

TEMPLATE = lib

CONFIG += c++11

LIBS +=     ../STT_Basic_Moudle/Communication/PCOMM.LIB \
            -lwsock32  -lws2_32

INCLUDEPATH += ../STT_GUI/stt_interface  \
               ../STT_GUI/stt_interface/interface

DEFINES += STT_BASIC_MOUDLE_LIBRARY


SOURCES += stt_basic_moudle.cpp \
    module.cpp \
    Communication/CCommuFactory.cpp \
    Communication/Communication.cpp \
    Communication/CommuPipe.cpp \
    Communication/CommuSerial.cpp \
    Communication/CommuTelnet.cpp \
    Communication/Telnet.cpp \
    Communication/WSOCKET.cpp \
    cmu.cpp \
    Communication/cping.cpp

HEADERS += stt_basic_moudle.h \
    Communication/CommuBase.h \
    Communication/Communication.h \
    Communication/CommuPipe.h \
    Communication/CommuSerial.h \
    Communication/CommuTelnet.h \
    Communication/Pcomm.h \
    Communication/Telnet.h \
    Communication/WSOCKET.h \
    cmu.h \
    Communication/cping.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    Communication/C.rc

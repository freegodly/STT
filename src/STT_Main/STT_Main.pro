#-------------------------------------------------
#
# Project created by QtCreator 2014-09-16T19:58:38
#
#-------------------------------------------------

QT       += core

QT       -= gui

CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ./3rd/lua  \
               ./stt_interface/interface

LIBS +=  -lwsock32  #../STT_Main/Communication/PCOMM.LIB   \


TARGET = STT_Main
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    3rd/lua_tinker/lua_tinker.cpp \
    stt_global.cpp \
    3rd/lua/lapi.c \
    3rd/lua/lauxlib.c \
    3rd/lua/lbaselib.c \
    3rd/lua/lcode.c \
    3rd/lua/ldblib.c \
    3rd/lua/ldebug.c \
    3rd/lua/ldo.c \
    3rd/lua/ldump.c \
    3rd/lua/lfunc.c \
    3rd/lua/lgc.c \
    3rd/lua/linit.c \
    3rd/lua/liolib.c \
    3rd/lua/llex.c \
    3rd/lua/lmathlib.c \
    3rd/lua/lmem.c \
    3rd/lua/loadlib.c \
    3rd/lua/lobject.c \
    3rd/lua/lopcodes.c \
    3rd/lua/loslib.c \
    3rd/lua/lparser.c \
    3rd/lua/lstate.c \
    3rd/lua/lstring.c \
    3rd/lua/lstrlib.c \
    3rd/lua/ltable.c \
    3rd/lua/ltablib.c \
    3rd/lua/ltm.c \
    3rd/lua/lua.c \
    3rd/lua/lundump.c \
    3rd/lua/lvm.c \
    3rd/lua/lzio.c \
    3rd/lua/print.c \
    3rd/lua/luac.c

HEADERS += \
    3rd/lua_tinker/lua_tinker.h \
    stt_interface/Basic_Module_Interface.h \
    stt_global.h \
    3rd/lua/lapi.h \
    3rd/lua/lauxlib.h \
    3rd/lua/lcode.h \
    3rd/lua/ldebug.h \
    3rd/lua/ldo.h \
    3rd/lua/lfunc.h \
    3rd/lua/lgc.h \
    3rd/lua/llex.h \
    3rd/lua/llimits.h \
    3rd/lua/lmem.h \
    3rd/lua/lobject.h \
    3rd/lua/lopcodes.h \
    3rd/lua/lparser.h \
    3rd/lua/lstate.h \
    3rd/lua/lstring.h \
    3rd/lua/ltable.h \
    3rd/lua/ltm.h \
    3rd/lua/lua.h \
    3rd/lua/luaconf.h \
    3rd/lua/lualib.h \
    3rd/lua/lundump.h \
    3rd/lua/lvm.h \
    3rd/lua/lzio.h

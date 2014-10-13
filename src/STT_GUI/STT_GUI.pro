#-------------------------------------------------
#
# Project created by QtCreator 2014-09-26T15:41:02
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = STT_GUI
TEMPLATE = app

CONFIG += c++11

#LIBS += ../STT_GUI/3rd/liblua.a

INCLUDEPATH +=  stt_interface/interface   \
                             subform  \
                             3rd/lua

SOURCES += main.cpp\
        mainwindow.cpp \
    form_loading.cpp \
    subform/form_testitem.cpp \
    subform/form_logshow.cpp \
    stt_global.cpp \
    subform/form_testinfo.cpp \
    subform/form_testblock.cpp \
    subform/form_teststack.cpp \
    3rd/lua_tinker/lua_tinker.cpp \
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
    3rd/lua/luac.c \
    3rd/lua/lundump.c \
    3rd/lua/lvm.c \
    3rd/lua/lzio.c \
    3rd/lua/print.c \
    subform/form_platformconfiguration.cpp \
    stt_test/stt_manage_thread.cpp \
    stt_test/stt_test_thread.cpp \
    subform/form_logtext.cpp \
    subform/form_scriptedit.cpp \
    subform/form_helper.cpp \
    subform/CodeEditor.cpp \
    subform/Highlighter.cpp


HEADERS  += mainwindow.h \
    form_loading.h \
    subform/form_testitem.h \
    subform/form_logshow.h \
    stt_global.h \
    subform/form_testinfo.h \
    subform/form_testblock.h \
    subform/form_teststack.h \
    stt_interface/Basic_Module_Interface.h \
    3rd/lua_tinker/lua_tinker.h \
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
    3rd/lua/lzio.h \
    subform/form_platformconfiguration.h \
    stt_test/stt_manage_thread.h \
    stt_test/stt_test_thread.h \
    subform/form_logtext.h \
    subform/form_scriptedit.h \
    subform/form_helper.h \
    subform/CodeEditor.h \
    subform/Highlighter.h


FORMS    += mainwindow.ui \
    form_loading.ui \
    subform/form_testitem.ui \
    subform/form_logshow.ui \
    subform/form_testinfo.ui \
    subform/form_testblock.ui \
    subform/form_teststack.ui \
    subform/form_platformconfiguration.ui \
    subform/form_logtext.ui \
    subform/form_scriptedit.ui \
    subform/form_helper.ui

RESOURCES += \
    res/res.qrc

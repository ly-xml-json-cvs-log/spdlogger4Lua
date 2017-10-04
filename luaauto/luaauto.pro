#-------------------------------------------------
#
# Project created by QtCreator 2017-09-28T13:41:25
#
#-------------------------------------------------

QT       -= core gui

TARGET = luaauto
TEMPLATE = lib

DEFINES += LUAAUTO_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        luaauto.cpp \
    ../luapp/luapp.cpp \
    auto.cpp \
    export.cpp \
    fun.cpp \
    luafun.cpp

HEADERS += \
        luaauto.h \
        luaauto_global.h \ 
    ../luapp/luapp.h \
    auto.h \
    export.h \
    fun.h \
    luafun.h

unix {
    target.path = /usr/local/lib
    INSTALLS += target
}

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = luaauto_global.h

INCLUDEPATH += /usr/local/include ../include
LIBS += -L/usr/local/lib -llua

CONFIG += c++14 shared

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.13

QMAKE_CXXFLAGS += -fvisibility=hidden
#QMAKE_CXXFLAGS -= -mmacosx-version-min=10.9
#QMAKE_LFLAGS -= -mmacosx-version-min=10.9
#QMAKE_LFLAGS += -mmacosx-version-min=10.12
#QMAKE_LFLAGS += -shared -fPIC

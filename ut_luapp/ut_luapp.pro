TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ut_luapp1.cpp \
    ../luapp/luapp.cpp

HEADERS += \
    ut_luapp_global.h \
    ../luapp/luapp.h

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ut_luapp_global.h

INCLUDEPATH += /usr/local/include ../include
LIBS += -L/usr/local/lib -llua

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.13

#QMAKE_CXXFLAGS -= -mmacosx-version-min=10.9
#QMAKE_CXXFLAGS += -fvisibility=hidden
#QMAKE_LFLAGS -= -mmacosx-version-min=10.9
#QMAKE_LFLAGS += -mmacosx-version-min=10.12
#QMAKE_LFLAGS += -shared -fPIC

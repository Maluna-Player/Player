#-------------------------------------------------
#
# Project created by QtCreator 2015-04-26T20:28:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Player
TEMPLATE = app


SOURCES += main.cpp\
    PlayerWindow.cpp \
    FmodManager.cpp

HEADERS  += PlayerWindow.h \
    LibException.h \
    BaseException.h \
    Constants.h \
    FmodManager.h


# Removal of warnings
QMAKE_CXXFLAGS_WARN_ON += -Wno-comment
QMAKE_CXXFLAGS_WARN_ON += -Wno-ignored-qualifiers


# Add of fmodex library

win32 {             # Window's paths
    INCLUDEPATH += "C:\Program Files (x86)\FMOD SoundSystem\FMOD Programmers API Windows\api\inc"
    LIBS += -L"C:\Program Files (x86)\FMOD SoundSystem\FMOD Programmers API Windows\api\lib"
    LIBS += -lfmodex
} else {            # Linux
    #INCLUDEPATH +=
    #LIBS +=
}

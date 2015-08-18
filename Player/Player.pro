#-------------------------------------------------
#
# Project created by QtCreator 2015-04-26T20:28:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Player
TEMPLATE = app


SOURCES += main.cpp\
    Tools.cpp \
    Gui/AboutDialog.cpp \
    Gui/ClickableLabel.cpp \
    Gui/PlayerButton.cpp \
    Gui/PlayerLabel.cpp \
    Gui/PlayerWindow.cpp \
    Gui/ProgressBackground.cpp \
    Gui/ProgressBar.cpp \
    Gui/SongList.cpp \
    Gui/Spectrum.cpp \
    Gui/VolumeViewer.cpp \
    Network/PlayerSocket.cpp \
    Network/RemoteSong.cpp \
    Audio/FmodManager.cpp \
    Audio/Player.cpp \
    Audio/Song.cpp \
    Network/PlayerMessage.cpp \
    Network/SongListItem.cpp \
    Network/Commands/Command.cpp \
    Network/Commands/CommandReply.cpp \
    Network/Commands/CommandRequest.cpp \
    Gui/ConnectionBox.cpp

HEADERS  += Constants.h \
    Tools.h \
    Gui/AboutDialog.h \
    Gui/ClickableLabel.h \
    Gui/PlayerButton.h \
    Gui/PlayerLabel.h \
    Gui/PlayerWindow.h \
    Gui/ProgressBackground.h \
    Gui/ProgressBar.h \
    Gui/SongList.h \
    Gui/Spectrum.h \
    Gui/VolumeViewer.h \
    Network/PlayerSocket.h \
    Network/RemoteSong.h \
    Exceptions/ArrayAccessException.h \
    Exceptions/BaseException.h \
    Exceptions/FileLoadingException.h \
    Exceptions/LibException.h \
    Audio/FmodManager.h \
    Audio/Player.h \
    Audio/Song.h \
    Network/PlayerMessage.h \
    Network/Sendable.h \
    Network/SongListItem.h \
    Network/Commands/Command.h \
    Network/Commands/CommandReply.h \
    Network/Commands/CommandRequest.h \
    Gui/ConnectionBox.h

RC_FILE = Player.rc

# Removal of warnings
QMAKE_CXXFLAGS_WARN_ON += -Wno-comment
QMAKE_CXXFLAGS_WARN_ON += -Wno-ignored-qualifiers


# Add of fmodex library

win32 {             # Window's paths
    INCLUDEPATH += "C:\Program Files (x86)\FMOD SoundSystem\FMOD Programmers API Windows\api\inc"
    LIBS += -L"C:\Program Files (x86)\FMOD SoundSystem\FMOD Programmers API Windows\api\lib"
    LIBS += -lfmodex
} else {            # Linux
    INCLUDEPATH += /usr/local/include/fmodex
    LIBS += -L/usr/local/lib/fmodex -lfmodex64
}

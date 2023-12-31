#-------------------------------------------------
#
# Project created by QtCreator 2023-07-17T16:22:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChineseChessTheClient
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    chess.cpp \
    chessarea.cpp \
    gamecenter.cpp \
    loginwindow.cpp \
    registerdialog.cpp \
    user.cpp \
    maininterface.cpp

HEADERS += \
        mainwindow.h \
    chess.h \
    chessarea.h \
    config.h \
    gamecenter.h \
    loginwindow.h \
    registerdialog.h \
    user.h \
    maininterface.h

FORMS += \
        mainwindow.ui \
    chess.ui \
    chessarea.ui \
    gamecenter.ui \
    loginwindow.ui \
    registerdialog.ui \
    maininterface.ui

RESOURCES += \
    res.qrc

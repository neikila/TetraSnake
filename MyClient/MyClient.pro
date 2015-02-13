#-------------------------------------------------
#
# Project created by QtCreator 2014-11-27T12:52:01
#
#-------------------------------------------------

QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyClient
TEMPLATE = app


SOURCES += main.cpp\
        myclient.cpp \
mainwindow.cpp \
    tinystr.cpp\
        tinyxml.cpp\
        tinyxmlerror.cpp\
        tinyxmlparser.cpp\
        NonClassFunction.cpp\
        Painter.cpp\
        Point.cpp\
        Snake.cpp\
        Heap.cpp \
        Figure.cpp\
        Player.cpp\
        Settings.cpp

HEADERS  += myclient.h \
        Point.h\
        tinyxml.h\
        tinystr.h\
        NonClassFunction.h\
        Painter.h\
        Point.h\
        Game.h\
        Player.h\
        Settings.h

FORMS    += myclient.ui

OTHER_FILES += \
    settings.xml\
                ClientSettings.xml

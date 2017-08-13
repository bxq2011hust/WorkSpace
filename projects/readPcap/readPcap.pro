#-------------------------------------------------
#
# Project created by QtCreator 2016-09-24T20:37:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = readPcap
TEMPLATE = app

LIBS += -lws2_32  \

SOURCES += src/main.cpp\
        src/mainwindow.cpp \


HEADERS  += src/mainwindow.h \
    src/pcapstruct.h \


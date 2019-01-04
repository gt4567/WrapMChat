#-------------------------------------------------
#
# Project created by QtCreator 2018-11-06T09:29:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testMChatDemo
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    Log/lclog.cpp

HEADERS  += widget.h \
    Log/lclog.h

FORMS    += widget.ui

LIBS += -L. -lqrencode
LIBS += -L. -llibzbar-0

INCLUDEPATH += $$PWD\qrcode\qrencode
INCLUDEPATH += $$PWD\qrcode\include

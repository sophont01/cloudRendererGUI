#-------------------------------------------------
#
# Project created by QtCreator 2016-02-10T18:27:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cloudRenderer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    crclient.cpp \
    renderscreen.cpp

HEADERS  += mainwindow.h \
    crclient.h \
    renderscreen.h

FORMS    += mainwindow.ui \
    renderscreen.ui

unix|win32: LIBS += -lboost_system

unix|win32: LIBS += -lboost_thread

unix|win32: LIBS += -lturbojpeg

#-------------------------------------------------
#
# Project created by QtCreator 2018-01-11T18:09:16
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += multimediawidgets
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyMediaPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mediacontroll.cpp \
    serallport.cpp

HEADERS  += mainwindow.h \
    mediacontroll.h \
    serallport.h

FORMS    += mainwindow.ui

RESOURCES += \
    icon.qrc

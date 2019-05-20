#-------------------------------------------------
#
# Project created by QtCreator 2019-05-07T10:30:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fire
TEMPLATE = app

QT+= serialport

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH+= "D:\opencv\install\include"

release
{
LIBS+= -LD:\opencv\lib\
-llibopencv_world320
}

debug
{
LIBS+= -LD:\opencv\lib\
-llibopencv_world320d
}

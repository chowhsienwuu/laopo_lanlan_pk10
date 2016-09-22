#-------------------------------------------------
#
# Project created by QtCreator 2016-07-31T22:25:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = laopo
TEMPLATE = app

CONFIG += c++11
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    textexamples.h

FORMS    += mainwindow.ui

DISTFILES += \
    love.rc \
    changelog.txt

RESOURCES += \
    launch.qrc
RC_FILE = love.rc


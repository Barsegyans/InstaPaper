#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T14:15:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
VERSION = 1.0
QMAKE_TARGET_COMPANY = SerBar Co
QMAKE_TARGET_PRODUCT = InstaPaper
QMAKE_TARGET_DESCRIPTION = text-processing program
QMAKE_TARGET_COPYRIGHT = Sergey Barsegyan
TARGET = TextEditor
TEMPLATE = app
RC_FILE = icons/appicon.rc

SOURCES += main.cpp\
        mainwindow.cpp
HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    img.qrc

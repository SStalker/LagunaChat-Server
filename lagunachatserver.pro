TEMPLATE = app
TARGET = LagunaChatServer
DEPENDPATH += .
INCLUDEPATH += .


CONFIG += console

QT += core network sql
QT -= gui
HEADERS += \
    sqlserver.h \
    LagunaChatServer.h
SOURCES += main.cpp \
    sqlserver.cpp \
    LagunaChatServer.cpp



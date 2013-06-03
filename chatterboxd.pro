TEMPLATE = app
TARGET = ChatterBoxServer
DEPENDPATH += .
INCLUDEPATH += .


CONFIG += console

QT += core network sql
QT -= gui
HEADERS += ChatterBoxServer.h \
    userlisttext.h \
    sqlserver.h
SOURCES += ChatterBoxServer.cpp main.cpp \
    userlisttext.cpp \
    sqlserver.cpp



QT += core network
QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = CalcServer
TEMPLATE = app

SOURCES += \
    main.cpp \
    server.cpp \
    calculator.cpp

HEADERS += \
    server.h \
    calculator.h
QT += core testlib
QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = CalcTests
TEMPLATE = app

INCLUDEPATH += $$PWD/../CalcServer

SOURCES += \
    test_calculator.cpp \
    $$PWD/../CalcServer/calculator.cpp

HEADERS += \
    $$PWD/../CalcServer/calculator.h
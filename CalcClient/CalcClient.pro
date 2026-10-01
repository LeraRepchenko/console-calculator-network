QT += core network
QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = CalcClient
TEMPLATE = app

SOURCES += \
    main.cpp \
    client.cpp

HEADERS += \
    client.h
QT += core
QT -= gui

TARGET  = vna_setup
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

include(../core/core.pri)
include(../rsaToolbox/rsatoolbox.pri)
SOURCES += main.cpp

HEADERS +=

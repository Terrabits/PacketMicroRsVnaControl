QT += core
QT -= gui

TARGET = measure
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

include(../RsaToolbox/rsatoolbox.pri)
SOURCES += main.cpp

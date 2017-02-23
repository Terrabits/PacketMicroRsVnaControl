QT += core
QT -= gui

TARGET = calibrate
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

include(../RsaToolbox/rsatoolbox.pri)
SOURCES += main.cpp

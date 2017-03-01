QT += core
QT -= gui

TARGET = vna_measure
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

include(../RsaToolbox/rsatoolbox.pri)
SOURCES += main.cpp

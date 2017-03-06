#-------------------------------------------------
#
# Project created by QtCreator 2017-03-05T16:56:15
#
#-------------------------------------------------

QT      += widgets network

TARGET   = core
DESTDIR  = $$PWD
TEMPLATE = lib
CONFIG  += staticlib
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)

include(../rsaToolbox/rsatoolbox.pri)
SOURCES += core.cpp
HEADERS += core.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    core.pri

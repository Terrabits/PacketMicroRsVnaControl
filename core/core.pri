

include($$PWD/../RsaToolbox/rsatoolbox.pri)
INCLUDEPATH += $$PWD/
DEPENDPATH  += $$PWD/


win32 {
    # C++11 enum class warnings:
    QMAKE_CXXFLAGS += /wd4482

    LIBS += -L$$PWD
    CONFIG(debug, release|debug) {
        LIBS += -l$$PWD/cored
    }
    else {
        LIBS += -l$$PWD/core
    }
}
macx {
    QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
    LIBS += -stdlib=libc++
    LIBS += -L$$PWD
    CONFIG(debug, release|debug) {
        LIBS += $$PWD/libcored.a
    }
    else {
        LIBS += $$PWD/libcore.a
    }
}

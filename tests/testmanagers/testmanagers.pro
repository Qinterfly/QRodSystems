LIB_PATH = $${PWD}/../../lib
ADS_PATH = $${LIB_PATH}/ADS

QT += testlib gui widgets
CONFIG += qt warn_on depend_includepath testcase

CONFIG += c++20
QMAKE_CXXFLAGS += -std=c++20

TEMPLATE = app

SOURCES +=  \
    testmanagers.cpp

LIBS += -L$${LIB_PATH}

include(../../src/core/core.pri)
include(../../src/managers/managers.pri)
include(../../lib/ADS/ads.pri)

INCLUDEPATH += ../../src
INCLUDEPATH += ../../lib

RESOURCES += \
    ../../resources/icons/icons.qrc \
    ../../resources/styles/styles.qrc

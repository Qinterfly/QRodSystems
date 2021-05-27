LIB_PATH = $${PWD}/../../lib
ADS_PATH = $${LIB_PATH}/ADS

QT += testlib gui widgets
CONFIG += qt warn_on depend_includepath testcase

CONFIG += c++20
unix: QMAKE_CXXFLAGS += -std=c++20
else: win32: QMAKE_CXXFLAGS += /std:c++latest

TEMPLATE = app

SOURCES +=  \
    testcentral.cpp

LIBS += -L$${LIB_PATH}

include(../../src/core/core.pri)
include(../../src/central/central.pri)
include(../../src/models/models.pri)
include(../../src/managers/managers.pri)
include(../../src/render/render.pri)
include(../../lib/ADS/ads.pri)

INCLUDEPATH += ../../src
INCLUDEPATH += ../../lib

RESOURCES += \
    ../../resources/icons/icons.qrc \
    ../../resources/styles/styles.qrc

# Distribution options
PROJECT_AUTHOR = "Qinterfly"
PROJECT_NAME = "QRodSystems"
PROJECT_VERSION = "TEST"

DEFINES += \
    APP_AUTHOR=\\\"$$PROJECT_AUTHOR\\\" \
    APP_NAME=\\\"$$PROJECT_NAME\\\" \
    APP_VERSION=\\\"$$PROJECT_VERSION\\\" \

LIB_PATH = $${PWD}/../lib
ADS_PATH = $${LIB_PATH}/ADS

QT  += core gui widgets

TARGET   = QRodSystems
TEMPLATE = app

CONFIG += c++20
unix: QMAKE_CXXFLAGS += -std=c++20
else: win32: QMAKE_CXXFLAGS += /std:c++latest
QMAKE_CXXFLAGS += -Wno-deprecated-enum-enum-conversion

include(core/core.pri)
include(main/main.pri)
include(central/central.pri)
include(models/models.pri)
include(managers/managers.pri)
include(render/render.pri)

# Deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Libraries
LIBS += -L$${LIB_PATH}
include($${ADS_PATH}/ads.pri) # Advanced Docking System

# Resources
RESOURCES += \
    ../resources/icons/icons.qrc \
    ../resources/styles/styles.qrc \
    ../resources/fonts/fonts.qrc

# Distribution options
PROJECT_AUTHOR = "Qinterfly"
PROJECT_NAME = "QRodSystems"
PROJECT_VERSION = "0.0.16"
PROJECT_DIST = $${PROJECT_NAME} (v$${PROJECT_VERSION})

DEFINES += \
    APP_AUTHOR=\\\"$$PROJECT_AUTHOR\\\" \
    APP_NAME=\\\"$$PROJECT_NAME\\\" \
    APP_VERSION=\\\"$$PROJECT_VERSION\\\" \

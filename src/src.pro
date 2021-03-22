LIB_PATH = $${PWD}/../lib
ADS_PATH = $${LIB_PATH}/ADS

QT     += core gui widgets

TARGET   = QRodSystems
TEMPLATE = app

CONFIG += c++20

DEFINES += QT_DEPRECATED_WARNINGS

include(core/core.pri)
include(main/main.pri)
include(managers/managers.pri)
include(render/render.pri)

# Deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Libraries
LIBS += -L$${LIB_PATH}

## Advanced Docking System
include($${ADS_PATH}/ads.pri)

RESOURCES += \
    ../resources/images.qrc

# Distribution options
PROJECT_NAME = QRodSystems
PROJECT_VERSION = 0.0.1
DISTNAME = $${PROJECT_NAME} (v$${PROJECT_VERSION})

DEFINES += \
    VERSION=\\\"$$PROJECT_VERSION\\\" \

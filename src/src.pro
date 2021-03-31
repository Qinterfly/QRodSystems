LIB_PATH = $${PWD}/../lib
ADS_PATH = $${LIB_PATH}/ADS

QT  += core gui widgets

TARGET   = QRodSystems
TEMPLATE = app

CONFIG += c++20

DEFINES += QT_DEPRECATED_WARNINGS

include(core/core.pri)
include(main/main.pri)
include(central/central.pri)
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
    ../resources/images.qrc

# Distribution options
PROJECT_AUTHOR = "Qinterfly"
PROJECT_NAME = "QRodSystems"
PROJECT_VERSION = "0.0.3"
PROJECT_DIST = $${PROJECT_NAME} (v$${PROJECT_VERSION})

DEFINES += \
    APP_AUTHOR=\\\"$$PROJECT_AUTHOR\\\" \
    APP_NAME=\\\"$$PROJECT_NAME\\\" \
    APP_VERSION=\\\"$$PROJECT_VERSION\\\" \

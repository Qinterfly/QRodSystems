LIB_PATH = $${PWD}/../lib
ADS_PATH = $${LIB_PATH}/ADS

QT     += core gui widgets

TARGET   = QRodSystems
TEMPLATE = app

CONFIG += c++20

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main/main.cpp \
    main/mainWindow.cpp

HEADERS += \
    main/mainWindow.h

FORMS += \
    main/mainWindow.ui

# Deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Libraries
LIBS += -L$${LIB_PATH}

## Advanced Docking System
include($${ADS_PATH}/ads.pri)
adsBuildStatic {
    DEFINES += ADS_STATIC
}
lessThan(QT_MAJOR_VERSION, 6) {
    win32 {
        QT += axcontainer
    }
}
INCLUDEPATH += $${ADS_PATH}/src
DEPENDPATH  += $${ADS_PATH}/src

RESOURCES += \
    ../resources/images.qrc

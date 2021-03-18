LIB_PATH = $${PWD}/../lib
ADS_PATH = $${LIB_PATH}/ADS

QT     += core gui widgets

TARGET   = QRodSystems
TEMPLATE = app

CONFIG += c++20

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main/controltabs.cpp \
    main/main.cpp \
    main/mainwindow.cpp \
    main/view3dwidget.cpp

HEADERS += \
    main/controltabs.h \
    main/mainwindow.h \
    main/view3dwidget.h

FORMS += \
    main/mainwindow.ui

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
INCLUDEPATH += $${ADS_PATH}/src
DEPENDPATH  += $${ADS_PATH}/src

RESOURCES += \
    ../resources/images.qrc

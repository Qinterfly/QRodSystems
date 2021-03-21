LIB_PATH = $${PWD}/../lib
ADS_PATH = $${LIB_PATH}/ADS

QT     += core gui widgets

TARGET   = QRodSystems
TEMPLATE = app

CONFIG += c++20

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    core/abstractdataobject.cpp \
    core/array.cpp \
    core/project.cpp \
    main/controltabs.cpp \
    main/logwidget.cpp \
    main/main.cpp \
    main/mainwindow.cpp \
    main/view3d.cpp \
    managers/dataobjectsmanager.cpp

HEADERS += \
    core/abstractdataobject.h \
    core/array.h \
    core/project.h \
    main/controltabs.h \
    main/logwidget.h \
    main/mainwindow.h \
    main/view3d.h \
    managers/dataobjectsmanager.h

FORMS += \
    main/mainwindow.ui \
    managers/dataobjectsmanager.ui

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

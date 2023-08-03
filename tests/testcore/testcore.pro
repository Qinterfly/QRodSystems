QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

CONFIG += c++latest
QMAKE_CXXFLAGS += -std=c++20
unix: QMAKE_CXXFLAGS += -Wno-deprecated-enum-enum-conversion

TEMPLATE = app

SOURCES += \
    testcore.cpp

include(../../src/core/core.pri)

INCLUDEPATH += ../../src
